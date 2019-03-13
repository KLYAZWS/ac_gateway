#include "udp_client.h"
#include <unistd.h>
#include <errno.h>

void *udp_client_handler(void *arg)
{
    int ret;
    int on = 1;
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(8003);
 //   sockaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    sockaddr.sin_addr.s_addr = inet_addr("192.168.8.106");
    //socket
    int udp_client_fd = socket(PF_INET,SOCK_DGRAM,0);
    if(udp_client_fd < 0)
    {
        perror("udp client socket err!\n");
    }
    else
    {
        printf("udp client ok\n");
    }
    ret = setsockopt(udp_client_fd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
    if(ret!=0)
    {
        printf("setsockopt SO_BROADCAST error:%d, %s\n", errno, strerror(errno));
        close(udp_client_fd);
    }

    ret = setsockopt(udp_client_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret!=0)
    {
        printf("setsockopt SO_REUSEADDR error:%d, %s\n", errno, strerror(errno));
        close(udp_client_fd);
    }
    //send
    while(1)
    {
		char buf[24] = {1,3,5,7,9,11};
        int ret = sendto(udp_client_fd,buf,sizeof(buf),0,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
        if(ret < 0)
        {
           perror("udp sendto err\n");
        }
        else
        {
            printf("sendo ok\n");
        }
        sleep(3);
    }
    //close
    close(udp_client_fd);
}

