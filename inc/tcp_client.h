#ifndef __TCP_CLIENT_H_
#define __TCP_CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "global.h"
#include <arpa/inet.h>

void *tcp_client_handler(void *arg);

#endif
