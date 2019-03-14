/**
 * @file tcp_client.c
 * @author kly
 * @brief
 * @version 0.1
 * @date 2019-03-13
 * @copyright Copyright (c) 2018 kuaikuaikeji.Co.Ltd. All rights reserved
 *                  修改记录
 * 0.1        kly        2019-03-13      创建文件
 *
 */
#include "tcp_client.h"
#include <unistd.h>

#define TCP_CLIENT_PORT  18861

/**
 * @brief tcp client 初始化
 * 
 * @param port 端口号
 * @return int 
 */
int tcp_client_init(int port)
{
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(port);
 //   sockaddr.sin_addr.s_addr = inet_addr("192.168.1.93");
    sockaddr.sin_addr.s_addr = inet_addr("192.168.8.106");

    int clie_fd = socket(PF_INET,SOCK_STREAM,0);
    if(clie_fd < 0)
    {
        perror("socket ok!\n");
    }
    int ret = connect(clie_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
    if(ret < 0)
    {
        perror("connect error");
        close(clie_fd);
    }
    return clie_fd;
}

/**
 * @brief tcp client线程
 * 
 * @param arg 
 * @return void* 
 */
void *tcp_client_handler(void *arg)
{
    int tcp_client_fd;
    tcp_client_fd = tcp_client_init(TCP_CLIENT_PORT);

    //4.recv/send
    while(1)
    {
        char buf_s[24] = {0,2,4,6,8};                                                                   
        send(tcp_client_fd,buf_s,sizeof(buf_s),0);
        sleep(3);
    }
    //5.close
    close(tcp_client_fd);
}
