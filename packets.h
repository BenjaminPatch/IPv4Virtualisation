#ifndef PACKETS_H
#define PACKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <arpa/inet.h>

#include "data.h"

#define VER_HLEN 69 // version: 0100. headerLen: 0101 (no options)

typedef struct {
    uint8_t   ver_hlen;   /* Header version and length (dwords). */
    uint8_t   service;    /* Service type. */
    uint16_t  length;     /* Length of datagram (bytes). */
    uint16_t  ident;      /* Unique packet identification. */
    uint16_t  fragment;   /* Flags; Fragment offset. */
    uint8_t   timetolive; /* Packet time to live (in network). */
    uint8_t   protocol;   /* Upper level protocol (UDP, TCP). */
    uint16_t  checksum;   /* IP header checksum. */
    uint32_t  srcAddr;   /* Source IP address. */
    uint32_t  destAddr;  /* Destination IP address. */

} IpHdr;


typedef struct {
    IpHdr header;
    char* payload;
} IpPack;


void construct_packet(IpHdr newPack, char* input);


#endif
