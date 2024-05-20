#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
void sys_err(char *str){
    perror(str);
    //exit(1);
}

int main()
{
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
    else printf("listen success\n");

    fd_set read_set;
    fd_set tmp;
    FD_ZERO(&read_set);
    FD_SET(lfd, &read_set);
    int maxfd = lfd;

    while(1){
        tmp = read_set;
        select(maxfd + 1, &tmp, NULL, NULL, NULL);

        for (int fd = 0; fd <= maxfd; fd++)
        {   
            if(fd == lfd && FD_ISSET(fd, &tmp)){ //监听的文件描述符可读
                struct sockaddr_in cliaddr;
                int clilen = sizeof(cliaddr);
                int cfd = accept(fd, (struct sockaddr *)&cliaddr, &clilen);
                if(cfd == -1) sys_err("accept err");
                FD_SET(cfd, &read_set);
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            // 寻找通信的文件描述符
            if (fd != lfd && FD_ISSET(fd, &tmp))
            {
                char recv_buf[4096] = {0};
                int len = recv(fd, recv_buf, sizeof(recv_buf), 0); // 接收数据
                if (len == -1)
                    sys_err("recv err");
                else if (len == 0)
                {
                    printf("client已断开连接\n");
                    FD_CLR(fd, &read_set);
                    close(fd);
                    break;
                }
                else if (len > 0)
                {
                    printf("recv buf: %s\n", recv_buf);
                    char send_buf[4096] = {0};
                    sprintf(send_buf, "hello client");
                    send(fd, send_buf, sizeof(send_buf), 0); // 发送数据，+1表示将 \0 也发送
                }
            }
        }
    }

    //6.断开连接
    close(lfd);
}
