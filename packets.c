#include "packets.h"

/**
 * Initiates all values in the IpHdr for the packet
 */
void construct_header(IpPack* newPack, struct Data* data, uint32_t dstIp,
        FragDetails fragDetails) {
    newPack->header.ver_hlen = VER_HLEN; // 69 == 0100 0101
    newPack->header.service = 0;
    newPack->header.length = htons(HEADER_LEN + strlen(newPack->payload));
    printf("length: %lu\n", (HEADER_LEN + strlen(newPack->payload)));
    newPack->header.ident = htons(data->idNum);
    if (!fragDetails.fragmented) {
        // Just set fragment and all it's flags to zero
        newPack->header.fragment = 0;
    } else {
        // Set the fragmentOffset and the flag that indicates this is the
        // last flag.
        newPack->header.fragment = fragDetails.fragmentOffset;
        newPack->header.fragment |= fragDetails.lastFrag << 13;
    }

    newPack->header.timetolive = 10;
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


/*
 * Sends a packet in either multiple fragments or one, depending on size of
 * payload.
 */
void construct_payload(struct Data* data, char* payload, 
        struct sockaddr_in receiverData, IpPack* newPack, uint32_t fakeIp) {
    int indexInPayload = 0;
    int indexInMTU = 0;
    char newPayload[data->MTU - 19];
    int sockfd;
    FragDetails fragDetails;
    memset(&fragDetails, 0, sizeof(FragDetails));
    fragDetails.fragmented = 0;
    fragDetails.fragmentOffset = 0;
    fragDetails.lastFrag = 0;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
    }
    while (indexInPayload < (int)strlen(payload)) {
        create_new_payload(data, newPayload, &indexInMTU, &indexInPayload, 
                payload);
        if (indexInMTU == (data->MTU - 20)) {
            fragDetails.fragmented = 1;
            fragDetails.lastFrag = 1;
        }
        memset(newPack->payload, 0, BUFFER);
        strcpy(newPack->payload, newPayload);
        construct_header(newPack, data, fakeIp, fragDetails);
        //printf("port sending to: %d\n", receiverData.sin_port);
        //printf("strlen: %d\n", (int)strlen(newPack->payload));
        if (sendto(sockfd, (void*)newPack, 20 + strlen(newPack->payload), 
                0, (struct sockaddr*)&receiverData, sizeof(receiverData)) == -1) {
            fprintf(stderr, "sendto\n");
        }
        fragDetails.fragmentOffset += (indexInMTU / 8);
        indexInMTU = 0;
    }
    return;
}


/**
 * Creates a new payload before sending it off with ip header as datagram
 */
void create_new_payload(struct Data* data, char* newPayload, int* indexInMTU,
        int* indexInPayload, char* payload) {
    memset(newPayload, 0, data->MTU - 19);
    while (*indexInMTU < data->MTU - 20) {
        // Add next char to new payload, iterate index on both indices
        newPayload[*indexInMTU] = payload[*indexInPayload];
        printf("indexInMTU: %d strlen(newPayload): %d   ", *indexInMTU, 
                (int)strlen(newPayload));
        printf(".  payload[*indexInPayload]: %c, indexInPayload: %d     ", 
                payload[*indexInPayload], *indexInPayload);
        printf("newPayload: %s\n", newPayload);
        
        *(indexInPayload) = *indexInPayload + 1;
        if (*indexInPayload >= (int)strlen(payload)) {
            // Send packet as is (less than MTU)
            break;
        }
        *(indexInMTU) = *indexInMTU + 1;
    }
    printf("indexInMTU: %d strlen(newPayload): %d\n", *indexInMTU, 
            (int)strlen(newPayload));
    printf("yeet: %d\n", newPayload[*(indexInMTU)]);
}
