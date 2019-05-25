#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "arp.h"
#include "packets.h"

#define BUFF 8192


int gw_set(char* input, struct Data* data);


int gw_get(struct Data* data);


int arp_set(char* input, struct Data* data);


int arp_get(char* input, struct Data* data);


int mtu_set(char* input, struct Data* data);


int mtu_get(struct Data* data, int returnMode);


void parse_ipAddr(struct Data* data, char* argv1);


int send_msg(struct Data* data, char* input);


void strip_quotes(char* section, char* payload);


void split_input(char* input, char** sections, int oneSpace, int len);


int is_in_subnet(struct Data* data, char* ip);

#endif 
