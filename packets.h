#ifndef PACKETS_H
#define PACKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>

#include "data.h"

#define VER_HLEN 69 // version: 0100. headerLen: 0101 (no options)
#define HEADER_LEN 20 // No options
#define BUFFER 8192
#define DEBUG_PACK 0

typedef struct {
    int fragmented;
    uint16_t fragmentOffset;
    int lastFrag;
} FragDetails;

typedef struct {
    uint8_t   ver_hlen;   /* Header version and length */
    uint8_t   service;    /* Service type. */
    uint16_t  length;     /* Length of datagram (bytes). */
    uint16_t  ident;      /* Unique packet identification. */
    uint16_t  fragment;   /* Flags; Fragment offset. */
    uint8_t   timetolive; /* Packet time to live */
    uint8_t   protocol;   /* Upper level protocol (UDP, TCP). */
    uint16_t  checksum;   /* IP header checksum. */
    uint32_t  srcAddr;   /* Source IP address. */
    uint32_t  destAddr;  /* Destination IP address. */

} IpHdr;


typedef struct {
    IpHdr header;
    char payload[BUFFER];
} IpPack;


/**
 * Initiates all values in the IpHdr for the packet
 */
void construct_header(IpPack* newPack, struct Data* data, uint32_t dstIp, 
        FragDetails fragDetails);


/**
 * Sends a packet in either multiple fragments or one, depending on size of
 * payload.
 */
void construct_payload(struct Data* data, char* payload, 
        struct sockaddr_in receiverData, IpPack* newPack, uint32_t fakeIp);


/**
 * Creates a new payload before sending it off with ip header as datagram
 */
void create_new_payload(struct Data* data, char* newPayload, int* indexInMTU,
        int* indexInPayload, char* payload);


#endif
