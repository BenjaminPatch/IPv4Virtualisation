#ifndef LISTEN_H
#define LISTEN_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "packets.h"

#define LONGEST_IP_PLUS_NULL 16

void* listen_for_message(void* data);


void non_zero_protocol(IpPack* incPack);


void zero_protocol(IpPack* incPack, int sockfd, struct Data* hostData);

#endif
