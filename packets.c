#include "packets.h"

void construct_packet(IpPack* newPack, struct Data* data, uint32_t dstIp) {
    newPack->header.ver_hlen = VER_HLEN; // 69 == 0100 0101
    newPack->header.service = 0;
    newPack->header.length = htons(20 + strlen(newPack->payload));
    newPack->header.ident = htons(data->idNum);
    newPack->header.fragment = 0;
    newPack->header.timetolive = 5;
    newPack->header.protocol = 0;
    newPack->header.checksum = 0;
    uint32_t srcAddr;
    if (inet_pton(AF_INET, data->hostIP, &(srcAddr)) == 0) {
        fprintf(stderr, "inet_pton() \n");
    }
    inet_pton(AF_INET, data->hostIP, &(newPack->header.srcAddr));
    newPack->header.srcAddr = srcAddr;
    newPack->header.destAddr = dstIp;
}
