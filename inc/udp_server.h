#ifndef __UDP_SERVER_H_
#define __UDP_SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "global.h"


int  udp_server_init(int port);
void pad_data_deal(int fd);
void slave_data_deal(int fd);
void *udp_server_handler(void *arg);

#endif

