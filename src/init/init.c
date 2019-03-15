/**
 * @file tcp_client.c
 * @author kly
 * @brief  AC应用，多线程
 * @version 0.1
 * @date 2019-03-13
 * @copyright Copyright (c) 2018 kuaikuaikeji.Co.Ltd. All rights reserved
 *                  修改记录
 * 0.1        kly        2019-03-13      创建文件
 *
 */
#include "init.h"
#include "tcp_client.h"
#include "udp_client.h"
#include "udp_server.h"


struct pthread thread;

int system_init(void)
{
    // pthread_create(&thread.thread_analysis,NULL,analysis_handler,NULL);
    pthread_create(&thread.thread_tcp_client,NULL,tcp_client_handler,NULL);
    pthread_create(&thread.thread_udp_client,NULL,udp_client_handler,NULL);
    pthread_create(&thread.thread_udp_server,NULL,udp_server_handler,NULL);
    // pthread_create(&thread.thread_recv_ap,NULL,recv_ap_handler,NULL);
}
