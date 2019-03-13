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
        perror("ftok");
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
            perror("msgget");
            exit(1);
        }
    }
    return msg_id;
}
