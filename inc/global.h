#ifndef __GLOBAL_HEAD_H_
#define __GLOBAL_HEAD_H_

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

/*协议命令类型*/
#define ORDER_GATEWAY_STATUS       0x41   //网关->PAD   状态命令
#define ORDER_PAD_HEARTBEAT        0x42   //PAD ->网关  心跳包命令
#define ORDER_PAD_BINDING          0x43   //PAD<->网关  绑定命令
#define ORDER_PAD_VERIFY_BINGDING  0x44	  //PAD ->网关  确认绑定命令
#define ORDER_PAD_CONFIGID         0x47	  //PAD<->网关  配置编号命令
#define ORDER_PAD_RESET            0x46   //PAD<->网关  重启命令
#define ORDER_PAD_STOP             0x25   //PAD<->网关  结束上课命令

#define ORDER_TYPE_UPLOAD          0x82   //上传命令
#define ORDER_TYPE_INTERRUPT       0x83   //中断上传命令

/*协议命令长度*/
#define LEN_ACKPAD            20   //网关应答Pad数据长度
#define LEN_ACKPAD_BINDING    22   //协议-应答数组
#define LEN_STATUS            133  //网关状态信息数组长度


#define LEN_INTERRUPT         70   //协议-中断上传数组长度
#define LEN_UPLOADDATA        960  //每次上传原始数据长度
#define LEN_UPLOAD_SUM        1024


#define FLASHSAVE_LEN         119  //flash存储空间的大小
#define GATEWAY_ID 			  100000001 //网关ID

#define NUM_KNEE              36   //膝带数量
#define UP_DATA_NUM           960  //存储数据

typedef unsigned char uchar;
typedef unsigned int  uint;

/* 线程 */
struct pthread
{
    pthread_t thread_tcp_client;    //tcp客户端，和云服务器通讯
    pthread_t thread_udp_server;    //udp服务器，pad通讯
    pthread_t thread_udp_client;    //udp客户端，pad通讯
    // pthread_t thread_recv_ap;       //接收ap数据
    // pthread_t thread_analysis;      //数据处理线程
};
 /*存储单个膝带节点数据*/
 typedef struct
 {
    uchar classID[8];
    uchar userID[4];
    uchar locaFlag;   //位置标识
    uchar deviceID[4];
    uchar node_mark;     //节点号
    uchar sla_data[2][UP_DATA_NUM+1]; //数据存储
    uchar fill_flag[2];     //写入标识
 }knee_typedef;

 /* 时间戳 */
 typedef struct
 {
    uchar  classStartTime[4];
    uchar  unit1EndTime[4];
    uchar  unit2EndTime[4];
    uchar  unit3EndTime[4];
    uchar  unit4EndTime[4];
    uchar  unit5EndTime[4];
    uchar  unit6EndTime[4];
}timestamp_typedef;

/*所有信息*/
typedef struct 
{
    knee_typedef kneeData[NUM_KNEE];
    timestamp_typedef timestamp;
}all_info_typedef;

/* PAD下发数据 */
typedef struct 
{ 
    uchar classID[8]; //课程ID
	uchar userID[4];  //用户ID
    uchar locaFlag;   //位置标识
	uchar deviceID[4];//设备ID
    uchar node_mark;     //节点号
    uchar gateway_ID[4];
    uchar element;      //单元数
    uchar UTC[4];       //时间戳
}pad_data_typedef;

#define USE_DEBUG

#ifdef USE_DEBUG
#define DEBUG_LINE() printf("[%s:%s] line=%d\r\n",__FILE__, __func__, __LINE__)
#define DEBUG_ERR(fmt, args...) printf("\033[46;31m[%s:%d]\033[0m "#fmt" errno=%d, %m\r\n", __func__, __LINE__, ##args, errno, errno)
#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%d]\033[0m "#fmt"\r\n", __func__, __LINE__, ##args)
#else
#define DEBUG_LINE()
#define DEBUG_ERR(fmt,...)
#define DEBUG(fmt,...) 
#endif

#endif




