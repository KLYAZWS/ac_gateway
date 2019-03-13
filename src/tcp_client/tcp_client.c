#include "tcp_client.h"
#include <unistd.h>


void *tcp_client_handler(void *arg)
{
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(18861);
 //   sockaddr.sin_addr.s_addr = inet_addr("192.168.1.93");
    sockaddr.sin_addr.s_addr = inet_addr("192.168.8.106");
    //1.socket
    printf("start\n");
    int clie_fd = socket(PF_INET,SOCK_STREAM,0);
    if(clie_fd < 0)
    {
        perror("socket ok!\n");
    }
#ifdef DEBUG
    printf("tcp socket ok!\n");
#endif
    //2.bind
    //3.connect
    int ret = connect(clie_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
    if(ret < 0)
    {
        perror("connect error");
        close(clie_fd);
    }
#ifdef DEBUG
    printf("tcp connect ok!\n");
#endif
    //4.recv/send
    while(1)
    {
        char buf_s[24] = {0,2,4,6,8};                                                                   
        send(clie_fd,buf_s,sizeof(buf_s),0);
        sleep(3);
    }
    //5.close
    close(clie_fd);
}
