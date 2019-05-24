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
    hostDetails.sin_port = hostData->hostLL;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket() in listen. errno: %s\n", strerror(errno));
    }

    if (bind(sockfd, (struct sockaddr *)&hostDetails, 
            sizeof(hostDetails)) < 0) {
        fprintf(stderr, "bind() in listen. errno: %s\n", strerror(errno));
    }
    incPack = (IpPack*)malloc(sizeof(IpPack));

    while (1) {
        memset(&senderDetails, 0, sizeof(senderDetails));
    
        memset(incPack, 0, sizeof(IpPack));
        int n;
        socklen_t len;
        n = recvfrom(sockfd, incPack, hostData->MTU, 0, 
                (struct sockaddr*) &senderDetails, &len);
        /*
        if (sem_wait(hostData->outputLock) != 0) {
            fprintf(stderr, "sem_wait errno: %s\n", strerror(errno));
        }
        */
    }
    return NULL;
}
