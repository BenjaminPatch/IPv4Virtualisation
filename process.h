#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arp.h"
#include "data.h"


int gw_set(char* input, struct Data* data);


int gw_get(struct Data* data);


int arp_set(char* input, struct Data* data);


int arp_get(char* input, struct Data* data);


void split_input(char* input, char** sections);


int mtu_set(char* input, struct Data* data);


int mtu_get(struct Data* data);


void parse_ipAddr(struct Data* data, char* argv1);

#endif 
