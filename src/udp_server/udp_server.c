
/**
 * @file udp_server.c
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
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define RECV_PAD_PORT   8001
#define RECV_SLAVE_PORT 2222

/**
 * @brief UDP服务器初始化
 * 
 * @param port 端口号
 * @return int 套接字描述符
 */
int  udp_server_init(int port)
{
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    //socket
    int udp_server_fd = socket(PF_INET,SOCK_DGRAM,0);
    if(udp_server_fd < 0)
    {
        perror("udp_server_fd  err!\n");
    }
    printf("udp_server_fd  ok\n");

    int ret = bind(udp_server_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
	if(ret < 0)
	{
		perror("bind error");
		close(udp_server_fd);
	}
	printf("udp server bind ok\n");
    return udp_server_fd;
}
/**
 * @brief 接收pad数据处理
 * 
 * @param fd 
 */
void pad_data_deal(int fd)
{
    int ret = 0;
    char buf[1024] = {0};
    uchar gatewayID[4] = {0};

    ret = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
    if(ret > 0)
    {
        printf("server recv ok\n");
    }
    switch (buf[0])
    {
        case ORDER_PAD_HEARTBEAT:
            gatewayID[0] = (GATEWAY_ID >> 24) & 0xff;/*网关ID*/
			gatewayID[1] = (GATEWAY_ID >> 16) & 0xff;
			gatewayID[2] = (GATEWAY_ID >> 8)  & 0xff;
			gatewayID[3] = (GATEWAY_ID) & 0xff;			
			if(!memcmp(&buf[1],gatewayID,4))	           //判断网关ID
			{
				memcpy(padSend.classID,&udpData[5],8);         //课程ID
				padSend.element = udpData[13];				   //单元
				memcpy(padSend.UTC,&udpData[14],4);			   //UTC
				db_Set_ElementTime(&padSend); //单元解析
				DEBUG("PAD send unit%d\r\n",padSend.element);
				
				return 4;
			}
			else
			{
				DEBUG("Gateway ID err\r\n");
			}
            break;
    
        default:
            break;
    }


}
/**
 * @brief 接收节点数据处理
 * 
 * @param fd 
 */
void slave_data_deal(int fd)
{
    int ret = 0;
    char buf[1024] = {0};
    memset(buf,0,sizeof(buf));

    ret = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
    if(ret > 0)
    {
        printf("server recv ok\n");
    }
    printf("recvfrom slave data:%s\n",buf);
}
/**
 * @brief UDP服务器线程
 * 
 * @param arg 
 * @return void* 
 */
void *udp_server_handler(void *arg)
{
    int i =0;
    int recv_pad_fd;    //接收pad数据
    int recv_slave_fd;  //接收节点数据
    recv_pad_fd = udp_server_init(RECV_PAD_PORT);
    recv_slave_fd = udp_server_init(RECV_SLAVE_PORT);
    printf("pad_fd:%d slave_fd:%d\n",recv_pad_fd,recv_slave_fd);

    /* 多路复用 */
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(recv_pad_fd,&rfds);
    FD_SET(recv_slave_fd,&rfds);

    int max_fd = recv_slave_fd;
    fd_set tmp;
    while(1)
    {
        tmp = rfds;
        printf("before select...\n");
        int ret = select(max_fd+1,&tmp,NULL,NULL,NULL);
        if(ret < 0)
        {
            perror("select error");
            continue;
        }
        for(i = 0;i <= max_fd;i++)
        {
            if(FD_ISSET(i,&tmp))
            {
                if(recv_pad_fd == i)
                {
                    pad_data_deal(recv_pad_fd);
                }
                else if(recv_slave_fd == i)
                {
                    slave_data_deal(recv_slave_fd);
                }
            }
        }
    }
    //close
    close(recv_pad_fd);
    close(recv_slave_fd);
}

