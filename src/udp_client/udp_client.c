/**
 * @file udp_client.c
 * @author kly
 * @brief
 * @version 0.1
 * @date 2019-03-13
 * @copyright Copyright (c) 2018 kuaikuaikeji.Co.Ltd. All rights reserved
 *                  修改记录
 * 0.1        kly        2019-03-13      创建文件
 *
 */
#include "udp_client.h"
#include <unistd.h>
#include <errno.h>

#define SEND_PAD_PORT    8003
#define SEND_SLAVE_PORT  2223

/**
 * @brief UDP client初始化
 * 
 * @param port 
 * @return int 
 */
 int udp_client_init(int port,struct sockaddr_in* sockaddr)
{
    int ret = 0;
    int on = 1;
    int udp_client_fd;
    sockaddr->sin_family = PF_INET;
    sockaddr->sin_port = htons(port);
    //sockaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    sockaddr->sin_addr.s_addr = inet_addr("192.168.8.106");

    udp_client_fd = socket(PF_INET,SOCK_DGRAM,0);
    if(udp_client_fd < 0)
    {
        DEBUG_ERR("udp client socket err!\n");
    }

    ret = setsockopt(udp_client_fd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
    if(ret!=0)
    {
        DEBUG_INFO("setsockopt SO_BROADCAST error:%d, %s\n", errno, strerror(errno));
        close(udp_client_fd);
    }

    ret = setsockopt(udp_client_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret!=0)
    {
        DEBUG_INFO("setsockopt SO_REUSEADDR error:%d, %s\n", errno, strerror(errno));
        close(udp_client_fd);
    }
    return udp_client_fd;
}

/**
 * @brief udp客户端线程
 * 
 * @param arg 
 * @return void* 
 */
void *udp_client_handler(void *arg)
{
    int send_pad_fd;
    struct sockaddr_in sockaddr;

    send_pad_fd = udp_client_init(SEND_PAD_PORT,&sockaddr);

    while(1)
    {
		char buf[24] = {1,3,5,7,9,11};
        int ret = sendto(send_pad_fd,buf,sizeof(buf),0,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
        if(ret < 0)
        {
           DEBUG_ERR("udp sendto err\n");
        }
        sleep(3);
    }
    //close
    close(send_pad_fd);
}

