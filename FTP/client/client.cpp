#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <string>

void sys_err(const char *str){
    perror(str);
    exit(1);
}

void msg_err(const char *str){
    std::cerr << str << std::endl;
    exit(1);
}

void read_cb(struct bufferevent *bev, void *ctx) {
    // 读取服务器端发送的数据
    struct evbuffer *input = bufferevent_get_input(bev);
    
    size_t len = evbuffer_get_length(input);
    char *data = new char[len + 1];
    evbuffer_remove(input, data, len);
    data[len] = '\0';
    
    std::cout << "Received: " << data;

    delete[] data;
}

void event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_CONNECTED)
    {
        std::cout << "Connected to server.\n";
        // 向服务器发送一些 FTP 命令
        const char *cmds[] = {
            // "USER along\r\n",
            // "PASS 654321\r\n",
            // "PORT 127,0,0,1,8,9\r\n",
            "PWD\r\n",
            // "LIST\r\n",
            // "CWD opt\r\n",
            // "CDUP\r\n",
            // "RETR hello.txt\r\n"
        };

        for (const char *cmd : cmds)
        {
            bufferevent_write(bev, cmd, strlen(cmd));
        };
    }
    else if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF))
    {
        std::cout << "Connection closed or error.\n";
        event_base_loopbreak(bufferevent_get_base(bev));
    }
}

int main() {
    // 初始化 Libevent
    struct event_base *base = event_base_new();
    if (!base) msg_err("Could not initialize Libevent!\n");

    // 创建 bufferevent 连接到 FTP 服务器
    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) msg_err("Could not create bufferevent!\n");

    // 发起连接
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // FTP 服务器地址
    sin.sin_port = htons(21); // FTP 默认端口

    if (bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        std::cerr << "Could not connect to server!\n";
        bufferevent_free(bev);
        event_base_free(base);
        return 1;
    }

     // 设置回调函数
    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL); 
    // 启用读写事件 
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    // 启动事件循环
    event_base_dispatch(base);

    // 清理资源
    bufferevent_free(bev);
    event_base_free(base);

    return 0;
}
