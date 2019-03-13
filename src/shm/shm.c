#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "global.h"

struct all_info *shm_init(void)
{
    key_t key;
    int shmid;
    all_info *shm_buf;
    if((key = ftok("/app",'s')) < 0)
    {
        perror("ftok");
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
            perror("shmget");
            exit(1);
        }
    }
    if((shm_buf = (struct all_info*)shmat(shmid,NULL,0)) == (void*)-1)
    {
        perror("shmat");
        exit(1);
    }
    return shm_buf;
}


