#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include "Thread_pool.h"
#include "DNS_msg.h"
#define DNSRELAY_PORT 53



int main() {
    //初始化socket
    {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket初始化失败\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNSRELAY_PORT);//使用默认端口，可修改在调试模式下能修改
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /*bind 函数将 server_addr 地址绑定到 sockfd 套接字上。
如果绑定失败,则返回 -1。
在这种情况下,程序会打印出错误信息 "bind" 并关闭套接字 sockfd，然后返回 1，表示程序执行失败。*/
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("绑定套接字失败\n");
        close(sockfd);
        return 1;
    }
    }



    thread_pool_t *pool = thread_pool_create();

    // 添加任务到线程池
    for (int i = 0; i < 100; i++) {

    }

    // 销毁线程池
    thread_pool_destroy(pool);

    return 0;
}