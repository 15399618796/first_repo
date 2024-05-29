#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

void sys_err(char *str){
    perror(str);
    exit(1);
}

int main(){
    //1.socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) sys_err("socket err");

    //2.connect
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    int ret = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret == -1) sys_err("connect err");

    //3.通信
    int cnt = 1;
    while(1){
        char send_buf[1024] = {0};
        sprintf(send_buf, "hello, i am client, pid = %d\n", getpid());
        write(fd, send_buf, strlen(send_buf));

        char recv_buf[1024] = {0};
        ret = read(fd, recv_buf, sizeof(recv_buf));
        if(ret == -1) sys_err("read err");
        else if(ret == 0) printf("server close\n");
        else printf("server say: %s\n", recv_buf); 
        //如果不包含\n，输出可能不会立即显示在终端上，因为输出通常是缓冲的。
        //加上\n的作用是刷新输出缓冲区，确保消息立即显示出来。

        sleep(2);
    }

    close(fd);

    return 0;
}
