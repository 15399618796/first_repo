#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
void sys_err(char *str){
    perror(str);
    //exit(1);
}

int main(){
    //1.创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1) sys_err("socket err");

    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666); //端口，要使用网络字节序
    addr.sin_addr.s_addr = INADDR_ANY; //0地址
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1) sys_err("bind err");

    //3.设置监听
    ret = listen(lfd, 128);
    if(ret == -1) sys_err("listen err");

    int epfd = epoll_create(100);
    if(epfd == -1) sys_err("epoll_create err");

    //------------设置epoll----------
    // 创建一个epoll_event对象，用来向epoll_ctl函数中添加
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    // 创建一个epoll_event对象数组，用来保存epoll_wait返回的就绪事件
    struct epoll_event evs[1024]; 
    int maxevents = sizeof(evs) / sizeof(evs[0]);
    while(1){
        int cnt = epoll_wait(epfd, evs, maxevents, -1);
        for(int i = 0; i < cnt; i++){
            int fd = evs[i].data.fd;
            if(fd == lfd){
                struct sockaddr_in cliaddr;
                int clilen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
                if(cfd == -1) sys_err("accept err");

                // 将cfd通信描述符加入到epoll中
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else{
                char recv_buf[4096] = {0};
                int len = recv(fd, recv_buf, sizeof(recv_buf), 0);
                if(len == -1)
                    sys_err("recv err");
                else if(len == 0){
                    printf("client断开连接\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                }
                else if(len > 0){
                    printf("recv buf: %s\n", recv_buf);
                    char send_buf[4096] = {0};
                    sprintf(send_buf, "hello client");
                    send(fd, send_buf, strlen(send_buf), 0);
                }
            }
        }
    }

    //6.断开连接
    close(lfd);
    //close(cfd);
}