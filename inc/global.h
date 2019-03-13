#ifndef __GLOBAL_HEAD_H_
#define __GLOBAL_HEAD_H_

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>

#define DEBUG  1
typedef unsigned char uchar;
typedef unsigned int  uint;

/* 线程 */
struct pthread
{
    pthread_t thread_tcp_client;    //tcp客户端，和云服务器通讯
    pthread_t thread_udp_server;    //udp服务器，pad通讯
    pthread_t thread_udp_client;    //udp客户端，pad通讯
    pthread_t thread_recv_ap;       //接收ap数据
    pthread_t thread_analysis;      //数据处理线程

};

 /*存储单个膝带节点数据*/
 typedef struct
 {
     uchar class_ID[8];
     uchar user_ID[4];
     uchar locat_flag; //位置标识
     uchar device_ID[4];
     uchar node_num;   //节点号
     uchar element;    //单元数
     uchar UTC[4];     //UTC时间戳
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

typedef struct 
{
    knee_typedef knee;
    timestamp_typedef timestamp;
}all_info;

#endif




