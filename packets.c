#include "packets.h"

void construct_packet(IpPack* newPack, char* input, struct Data* data) {
    newPack->header.ver_hlen = VER_HLEN;
    newPack->header.service = 0;
    newPack->header.length = htons(20 + sizeof(input));
    newPack->header.ident = htons(data->idNum);
    newPack->header.fragment = 0;
    newPack->header.timetolive = 15;
    newPack->header.protocol = 0;
    newPack->header.checksum = 0;
    newPack->header.srcAddr = data->ipAddr;
    
    newPack->payload = input
}
