#include "listen.h"

void* listen_for_message(void* data) {
    int sockfd;
    struct Data* hostData = (struct Data*)data;
    struct sockaddr_in hostDetails, senderDetails;
    IpPack* incPack;

    memset(&hostDetails, 0, sizeof(hostDetails));
    // Listen on port given via argv, on localhost.
    hostDetails.sin_family = AF_INET;
    hostDetails.sin_addr.s_addr = 0;
    hostDetails.sin_port = htons(hostData->hostLL);

    // Open IPv4 socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket() in listen. errno: %s\n", strerror(errno));
    }

    // Bind socket to localhost port "hostDetails->llAddr"
    if (bind(sockfd, (struct sockaddr *)&hostDetails, 
            sizeof(hostDetails)) < 0) {
        fprintf(stderr, "bind() in listen. errno: %s\n", strerror(errno));
    }
    incPack = (IpPack*)malloc(sizeof(IpPack));

    while (1) {
        int n;
        socklen_t len;
        len = sizeof(senderDetails);
        memset(&senderDetails, 0, sizeof(senderDetails));
        memset(incPack, 0, sizeof(IpPack));
        n = recvfrom(sockfd, incPack, hostData->MTU, 0, 
                (struct sockaddr*) &senderDetails, &len);
        if (n == 0) {
            fprintf(stderr, "recvfrom returned 0\n");
        }
        /*
        char* byteArray = (char*)incPack;
        for (size_t i = 0; i < 29; i++) {
            printf("%d ", byteArray[i]);
        }
        printf("ver_hlen: %u\n", incPack->header.ver_hlen);
        printf("service: %u\n", ntohs(incPack->header.service));
        printf("length: %u\n", ntohs(incPack->header.length));
        printf("protocol: %u\n", incPack->header.protocol);
        */
        // There are different print statements depending on the protocol no.
        if (incPack->header.protocol) {
            if (sem_wait(hostData->outputLock)) {
                fprintf(stderr, "sem_wait failed with errno: %s\n", 
                        strerror(errno));
            }
            non_zero_protocol(incPack);
        } else {
            zero_protocol(incPack);
        }
        if (sem_post(hostData->outputLock)) {
            fprintf(stderr, "sem_post failed with errno: %s\n", 
                    strerror(errno));
        }
    }
    return NULL;
}


void non_zero_protocol(IpPack* incPack) {
    char ip[LONGEST_IP_PLUS_NULL];
    struct in_addr inAddr;
    inAddr.s_addr = incPack->header.srcAddr;
    if (inet_ntop(AF_INET, &(inAddr), ip, INET_ADDRSTRLEN) == NULL) {
        fprintf(stderr, "inet_ntop failed with errno: %s\n", 
                strerror(errno));
    }

    
    printf("\b\bMessage received from %s with protocol 0x", ip);
    if (incPack->header.protocol < 15) {
        printf("0");
    }
    printf("%x\n> ", incPack->header.protocol);
}


void zero_protocol(IpPack* incPack) {
    char ip[INET_ADDRSTRLEN];
    struct in_addr inAddr;
    inAddr.s_addr = incPack->header.srcAddr;
    if (inet_ntop(AF_INET, &(inAddr), ip, INET_ADDRSTRLEN) == NULL) {
        fprintf(stderr, "inet_ntop failed with errno: %s\n", 
                strerror(errno));
    }
    
    printf("\b\bMessage received from %s: \"%s\"\n> ",
            ip, incPack->payload);
}



