#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>

void sys_err(char *str){
    perror(str);
    exit(1);
}

int main(){
    //1.创建通信的套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1) sys_err("socket err");

    //2.连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666); //端口
    inet_pton(AF_INET, "192.168.43.185", &addr.sin_addr.s_addr); //将ip地址转换成 大端
    int ret = connect(cfd, (struct sockaddr *)&addr, sizeof(addr)); //客户端连接服务器
    
    //3.进行通信
    int num = 1;
    while(1){
        char send_buf[4096];
        //发送数据
        sprintf(send_buf, "hello %d", num ++);
        int len = send(cfd, send_buf, sizeof(send_buf), 0);

        //接收数据
        char recv_buf[4096] = {0};
        len = recv(cfd, recv_buf, sizeof(recv_buf), 0);
        if(len == -1) sys_err("send err");
        else if(len == 0){
            printf("serve已断开连接\n");
            break;
        }else if(len > 0){
            printf("%s\n", recv_buf);
        }
        sleep(1);
    }

    //4.断开连接
    close(cfd);
}