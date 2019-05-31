#ifndef DATA_H
#define DATA_H

#include <semaphore.h>

#define LONGEST_IP_WITH_NULL 16
#define DEBUG 0

struct Data {
    char* gw; // Current gateway IP
    char hostIP[LONGEST_IP_WITH_NULL]; // Host IP as specified in argv
    int cidrLen; // Bit length of subnet
    int hostLL; // Host Link Layer address, specified in argv
    int MTU; // Max transferable Units
    struct Dict* arpMappings; /* A dictionary mapping various ip Addresses to
                               * link layer addresses (port nums) */
    int idNum; // ID of next datagram to send
    sem_t* outputLock;
};

#endif 
