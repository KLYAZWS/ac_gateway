/**
 * @file udp_client.c
 * @author kly
 * @brief 消息队列
 * @version 0.1
 * @date 2019-03-13
 * @copyright Copyright (c) 2018 kuaikuaikeji.Co.Ltd. All rights reserved
 *                  修改记录
 * 0.1        kly        2019-03-13      创建文件
 *
 */
#include "msg.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "global.h"

int msg_init(void)
{
    int msg_id;
    key_t key;
    key = ftok("/app",'m');
    if(key < 0)
    {
        DEBUG_ERR("ftok");
        exit(1);
    }
    msg_id = msgget(key,IPC_CREAT|IPC_EXCL|0777);
    if(-1 == msg_id)
    {
        if(errno == EEXIST)
        {
            msg_id = msgget(key,0);
        }
        else
        {
            DEBUG_ERR("msgget");
            exit(1);
        }
    }
    return msg_id;
}
