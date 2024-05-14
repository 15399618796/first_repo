#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "threadpool.h"

typedef struct ClientInfo{
    int fd;
    struct sockaddr_in addr;
    int id; //客户端id
}ClientInfo;

typedef struct PoolInfo{
    int fd;
    ThreadPool *pool;
}PoolInfo;

void sys_err(char *s){
    perror(s);
}

void accept_working(void *arg);
void working(void *arg);

int main(){
    //1.socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1) sys_err("socket erorr");

    //2.bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) sys_err("bind error");

    //3.listen
    ret = listen(lfd, 128);
    if(ret == -1) sys_err("listen error");
    else printf("listen success\n");

    //创建线程池 
    ThreadPool *pool = threadPoolCreate(3, 10, 100);
    // 因为这块内存我们在thradpool中会手动释放，所以申请堆内存，不用栈区内存
    PoolInfo *poolInfo = (PoolInfo *)malloc(sizeof(PoolInfo)); 
    poolInfo->fd = lfd;
    poolInfo->pool = pool;
    threadPoolAdd(pool, accept_working, poolInfo);

    pthread_exit(NULL);
    return 0;
}

void accept_working(void *arg){
    PoolInfo *poolInfo = (PoolInfo *)arg;
    //4.accept
    int cnt = 0;
    while (1)
    {//循环监听，每有一个i新的连接就创建一个子进程处理
        struct sockaddr_in cliaddr;
        int clilen = sizeof(cliaddr);
        int cfd = accept(poolInfo->fd, (struct sockaddr *)&cliaddr, &clilen);
        if (cfd == -1) sys_err("accept error");

        // 因为这块内存我们在thradpool中会手动释放，所以申请堆内存，不用栈区内存
        ClientInfo *clientInfo = (ClientInfo *)malloc(sizeof(ClientInfo)); 
        clientInfo->addr = cliaddr;
        clientInfo->fd = cfd;
        clientInfo->id = ++ cnt;
        threadPoolAdd(poolInfo->pool, working, clientInfo);
        // pthread_t tid;
        // pthread_create(&tid, NULL, (void *)working, (void *)&infos[cnt ++]);
        // pthread_detach(tid);
    }

    close(poolInfo->fd);
    //close(cfd);
}

void working(void *arg)
{
    ClientInfo *clientInfo = (ClientInfo *)arg;   
    char ip[16] = {0};
    printf("client ip: %s, port: %d\n", inet_ntop(AF_INET, &clientInfo->addr.sin_addr.s_addr, ip, sizeof ip), ntohs(clientInfo->addr.sin_port));
    // 5 通信
    while (1)
    {
        char recv_buf[1024] = {0};
        int ret = read(clientInfo->fd, recv_buf, sizeof(recv_buf));
        if (ret == -1)
            sys_err("read error");
        else if (ret == 0)
        {
            printf("client close\n");
            break;
        }
        else
        {
            printf("recv buf: %s", recv_buf);
            char send_buf[1024] = {0};
            sprintf(send_buf, "hello client%d", clientInfo->id);
            write(clientInfo->fd, send_buf, strlen(send_buf));
        }
    }
}
w
