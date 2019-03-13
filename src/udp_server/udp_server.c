#include "udp_client.h"
#include <unistd.h>

void *udp_server_handler(void *arg)
{
    struct sockaddr_in sockaddr;
    struct sockaddr_in clitaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_port = htons(8001);
    sockaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    //socket
    int udp_server_fd = socket(PF_INET,SOCK_DGRAM,0);
    if(udp_server_fd < 0)
    {
        perror("udp_server_fd  err!\n");
    }
    printf("udp_server_fd  ok\n");

    // int on = 1;
    // setsockopt(udp_server_fd,SOL_SOCKET,SO_REUSEADDR|SO_BROADCAST,&on,sizeof(on));
    int ret = bind(udp_server_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
	if(ret < 0)
	{
		perror("bind error");
		close(udp_server_fd);
	}
	printf("udp server bind ok\n");
    //recv
    while(1)
    {
        //recvfrom
	    char buf[1024] = {0};
	    memset(buf,0,sizeof(buf));
	    /*注意*/
	    socklen_t addr_len = sizeof(clitaddr);

	    ret = recvfrom(udp_server_fd,buf,sizeof(buf),0,(struct sockaddr*)&clitaddr,&addr_len);
	    if(ret > 0)
	    {
            printf("recvfrom clint:%s\n",buf);
	    }
    }
    //close
    close(udp_server_fd);
}

