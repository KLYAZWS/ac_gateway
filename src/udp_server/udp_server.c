/**
 * @file udp_server.c
 * @author kly
 * @brief UDP服务器
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
#include "analysis.h"

#define RECV_PAD_PORT   8001
#define RECV_SLAVE_PORT 2222

/**
 * @brief UDP服务器初始化
 * 
 * @param port 端口号
 * @return int 套接字描述符
 */
static int  udp_server_init(int port)
{
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    //socket
    int udp_server_fd = socket(PF_INET,SOCK_DGRAM,0);
    if(udp_server_fd < 0)
    {
        DEBUG_ERR("udp_server_fd  err!\n");
    }
    DEBUG_INFO("udp_server_fd  ok\n");

    int ret = bind(udp_server_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
	if(ret < 0)
	{
		DEBUG_ERR("bind error");
		close(udp_server_fd);
	}
	DEBUG_INFO("udp server bind ok\n");
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
    pad_data_typedef pad_send_data;
    ret = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
    if(ret > 0)
    {
        DEBUG_INFO("server recv ok\n");
    }
    switch (buf[0])
    {
        /* 心跳包 */
        case ORDER_PAD_HEARTBEAT:
            gatewayID[0] = (GATEWAY_ID >> 24) & 0xff;
			gatewayID[1] = (GATEWAY_ID >> 16) & 0xff;
			gatewayID[2] = (GATEWAY_ID >> 8)  & 0xff;
			gatewayID[3] = (GATEWAY_ID) & 0xff;			
			if(!memcmp(&buf[1],gatewayID,4))	        //判断网关ID
			{
				memcpy(pad_send_data.classID,&buf[5],8);//课程ID
				pad_send_data.element = buf[13];		//单元
				memcpy(pad_send_data.UTC,&buf[14],4);	//UTC
				set_elementTime(&pad_send_data);        //单元解析
				DEBUG_INFO("PAD send unit%d\r\n",pad_send_data.element);	
				// return 4;
			}
			else
			{
				DEBUG_INFO("Gateway ID err\r\n");
			}
            break;
        /*PAD课程结束*/
		case ORDER_PAD_STOP: 
            /*
		    onlinenumb = db_Get_onlineDev();
			for(uint8_t i = 1;i <= onlinenumb;i++)
			{
				db_Set_packetLossRate((1-((float)db_Get_fillFlag(i)/(float)LEN_KNEEDATA/(float)timeFlag.packetNum))*1000,i);
			}    		
            db_End_Class(&padSend);							
			DEBUG_INFO("PAD send class end.\r\n");
			
			return 5;
            */			
			break;
    	/*PAD绑定命令*/
		case ORDER_PAD_BINDING:   
			memcpy((void *)pad_send_data.classID,(void*)&buf[1],13);
			pad_send_data.node_mark = get_node_mark(&pad_send_data);
			DEBUG_INFO("pad send binding\r\n");
			// return 6;	
			break;
        /*PAD配置编号*/
        case ORDER_PAD_CONFIGID:     
			memcpy(pad_send_data.deviceID,&buf[1],4);
			DEBUG_INFO("pad send set number\r\n");
			// return 7;
			break;
        /*PAD确认绑定*/
		case ORDER_PAD_VERIFY_BINGDING:     		
			if(!memcmp((void *)pad_send_data.classID,(void*)&buf[5],17))//classID和userID和locaFlag和deviceID 均符合
			{
				set_bingding(&pad_send_data);
				DEBUG_INFO("pad verify binding ok\r\n");
			}
			else
			{
				DEBUG_INFO("verify bingding error!\r\n");
			}	
			// return 8;
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
        DEBUG_INFO("server recv ok\n");
    }
    DEBUG_INFO("recvfrom slave data:%s\n",buf);
}

/**
 * @brief udp服务器线程
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
    DEBUG_INFO("pad_fd:%d slave_fd:%d\n",recv_pad_fd,recv_slave_fd);

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
        DEBUG_INFO("before select...\n");
        int ret = select(max_fd+1,&tmp,NULL,NULL,NULL);
        if(ret < 0)
        {
            DEBUG_ERR("select error");
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

