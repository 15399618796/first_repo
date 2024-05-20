#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

void read_cb(struct bufferevent *bev, void *ctx){
    //读取来自服务器的数据，发送数据在send_cb函数中
    char recv_buf[1024] = {0};
    bufferevent_read(bev, recv_buf, sizeof(recv_buf));
    printf("this is read_cb\n");
    printf("recv_buf:%s\n", recv_buf);
}
//写回调函数一般不用
void write_cb(struct bufferevent *bev, void *ctx){
    printf("this is write_cb\n");
}
//事件回调函数，返回一些错误信息
void event_cb(struct bufferevent *bev, short events, void *ctx){
    if(events & BEV_EVENT_EOF)
    {
        printf("server connection closed\n");
    }
    else if(events & BEV_EVENT_ERROR){
        printf("error\n");
    }
    else if(events & BEV_EVENT_CONNECTED){
        printf("connected success\n");
        return ;
    }

    //出错，终止事件循环
    struct event_base *base = (struct event_base *)ctx;
    event_base_loopbreak(base);
}
//客户端主动发送数据的回调函数
void send_cb(int fd, short event, void *arg){
    char buf[1024] = {0};
    read(fd,buf, sizeof(buf)); //从标准输入设备读取数据
    //sprintf(buf, "hello, i am client");
    struct bufferevent *bev = (struct bufferevent *)arg; //利用bufferevent来通信
    bufferevent_write(bev, buf, sizeof(buf));
    printf("客户端发送完毕\n");
    //usleep(200);
}

int main(){
    // 1.创建事件处理框架
    struct event_base *base = event_base_new();
    // 2.创建自带缓冲区的事件，用于socket通信
    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    // 3.连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr); 
    bufferevent_socket_connect(bev, (struct sockaddr *)&addr, sizeof(addr));
    // 给bufferevent事件设置回调函数
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, base);
    //设置读缓冲区可用
    bufferevent_enable(bev, EV_READ);

    //客户端要主动发送数据
    //普通事件回调函数触发时机是在事件框架开始检测，bufferevent读回调触发时机是bufferevent缓冲区有数据可读
    struct event *ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, send_cb, bev);
    event_add(ev, NULL);


    // 4.启动事件处理框架
    event_base_dispatch(base);
    // 5.释放资源
    bufferevent_free(bev);
    event_base_free(base);
}
