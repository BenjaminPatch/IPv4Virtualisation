#ifndef ARP_H
#define ARP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LARGEST_PORT_LEN 6
#define LONGEST_IP_PLUS_NULL 16

struct Dict {
    void* val;
    struct Dict* next;
};

struct Arp {
    char ipAddr[LONGEST_IP_PLUS_NULL];
    char* llAddr;
};

void append_to_dict(struct Dict* dict, struct Arp* mapping);


void display_dict_entries(struct Dict* dict);


void findMapping(char* ip, struct Dict* dict);

#endif 
