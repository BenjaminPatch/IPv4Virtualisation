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

#include "data.h"
#include "packets.h"

void* listen_for_message(void* data);

#endif
