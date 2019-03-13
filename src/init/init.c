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
