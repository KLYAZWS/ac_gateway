#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "global.h"

static struct all_info* all_info;

void shm_init(void)
{
    key_t key;
    int shmid;
    if((key = ftok("/app",'s')) < 0)
    {
        DEBUG_ERR("ftok");
        exit(1);
    }
    if((shmid = shmget(key,sizeof(struct all_info),IPC_CREAT|IPC_EXCL|0666)) == -1)
    {
        if(errno == EEXIST)
        {
            shmid = shmget(key,0,0);
        }
        else
        {
            DEBUG_ERR("shmget");
            exit(1);
        }
    }
    if((all_info = (struct all_info*)shmat(shmid,NULL,0)) == (void*)-1)
    {
        DEBUG_ERR("shmat");
        exit(1);
    }
}

v



