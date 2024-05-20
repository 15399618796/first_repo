#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
//bufferevent读回调函数，处理主要通信流程
void read_cb(struct bufferevent *bev, void *ctx){
    char recv_buf[1024] = {0};
    int len = bufferevent_read(bev, recv_buf, sizeof(recv_buf));
    if(len == -1) printf("server recv error\n");
    else printf("recv_buf: %s", recv_buf);

    //char send_buf[2048] = {0}; 有莫名奇妙的bug
    char send_buf[1024] = {0};
    //sprintf(send_buf, "hello cilent, I am server, recv: %s", recv_buf);
    sprintf(send_buf, "hello client");
    len = bufferevent_write(bev, send_buf, sizeof(send_buf));
    if(len == -1) printf("server send error");
    else printf("已经给客户端回复:%s, %d\n", send_buf, len);
    //sleep(3);
}
void write_cb(struct bufferevent *bev, void *ctx){
    printf("this is write_cb\n\n");
}
//读写操作以外的事件回调函数
void event_cb(struct bufferevent *bev, short events, void *ctx)
{
    struct event_base *base = (struct event_base *)ctx;
    if(events & BEV_EVENT_EOF)
    {
        printf("client已断开\n");
    }
    else if(events & BEV_EVENT_ERROR)
    {
        printf("error\n");
    }

    //终止当前程序，停止事件循环
    event_base_loopbreak(base);
}

//链接监听器的回调函数，主要用于通信，封装fd, 让bufferevent帮助通信
void listener_cb(
    struct evconnlistener *listener,
	evutil_socket_t fd,
	struct sockaddr *addr,
	int len,
	void *ptr
)
{
    struct event_base *base = (struct event_base *)ptr;
    //封装通信描述符fd,创建带缓冲区的事件
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    // 设置bufferevent的回调函数
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, base);
    bufferevent_enable(bev, EV_READ); //设置读缓冲区可用
}

int main(){
    //1.创建事件处理框架
    struct event_base *base = event_base_new();
    //2.创建链接监听器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    addr.sin_addr.s_addr = INADDR_ANY;

    struct evconnlistener *evl = evconnlistener_new_bind(
        base, listener_cb, base,
        LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
        -1,
        (struct sockaddr*)&addr, sizeof(addr)
    );
    //3.启动事件循环
    event_base_dispatch(base);

    //4.释放资源
    event_base_free(base);
    evconnlistener_free(evl);
}
