#ifndef __UDP_SERVER_H_
#define __UDP_SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "global.h"


void *udp_server_handler(void *arg);

#endif

