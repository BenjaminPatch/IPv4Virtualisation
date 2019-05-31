#include "process.h"

int gw_set(char* input, struct Data* data) {
    if (!data->gw) {
        data->gw = (char*)malloc(sizeof(char) * LONGEST_IP_PLUS_NULL);
        memset(data->gw, 0, LONGEST_IP_PLUS_NULL);
        strcpy(data->gw, input + 7);
        strtok(data->gw, "\n"); // strip newline
    }

    return 0;
}

int gw_get(struct Data* data) {
    if (!data->gw) {
        sem_wait(data->outputLock);
        printf("None\n");
    } else {
        sem_wait(data->outputLock);
        printf("%s\n", data->gw);
    }

    return 0;
}

int arp_set(char* input, struct Data* data) {
    char** sections = (char**)malloc(sizeof(char*) * 3);
    int lenOfArpSet = 8; // Length of string "arp set "
    sections[0] = (char*)malloc(sizeof(char) * lenOfArpSet);
    sections[1] = (char*)malloc(sizeof(char) * LONGEST_IP_PLUS_NULL);
    sections[2] = (char*)malloc(sizeof(char) * LARGEST_PORT_LEN + 1);
    memset(sections[0], 0, lenOfArpSet);
    memset(sections[1], 0, LONGEST_IP_PLUS_NULL);
    memset(sections[2], 0, LARGEST_PORT_LEN);

    // Split input so we can easily access ipAddr and llAddr
    split_input(input, sections, 0);

    // Create arp struct based on the split input
    struct Arp* newArp = (struct Arp*)malloc(sizeof(struct Arp)); 
    memset(newArp, 0, sizeof(struct Arp));
    memset(newArp->ipAddr, 0, LONGEST_IP_PLUS_NULL);
    strcpy(newArp->ipAddr, sections[1]); //try strcpy
    newArp->llAddr = sections[2];
    append_to_dict(data->arpMappings, newArp);
    if (DEBUG)
        display_dict_entries(data->arpMappings);

    return 0;
}

int arp_get(char* input, struct Data* data) {
    if (data->arpMappings->val == NULL) {
        sem_wait(data->outputLock);
        printf("None\n");
    } else {
        const char s[2] = " ";
        char* tok = strtok(input, s);
        tok = strtok(0, s);
        tok = strtok(0, s);
        int cliMode = 1;
        findMapping(tok, data, cliMode);
    }

    return 0;
}




int mtu_set(char* input, struct Data* data) {
    strtok(input, "\n");
    data->MTU = atoi(input + 8);
    return 0;
}




int mtu_get(struct Data* data, int returnMode) {
    sem_wait(data->outputLock);
    if (returnMode) {
        return data->MTU;
    } else {
        printf("%d\n", data->MTU);
    }
    return 0;
}




void parse_ipAddr(struct Data* data, char* argv1) {
    const char delim[2] = "/";
    char* token = strtok(argv1, delim);
    memset(data->hostIP, 0, LONGEST_IP_PLUS_NULL);
    strcpy(data->hostIP, token);
    token = strtok(NULL, delim);
    data->cidrLen = atoi(token);
}




int send_msg(struct Data* data, char* input) {
    int port;
    uint32_t fakeIp;
    struct sockaddr_in receiverData;
    char** sections = (char**)malloc(sizeof(char*) * 3);
    memset(sections, 0, sizeof(char*) * 3);
    split_input(input, sections, 1);

    // Strip double-quotes 
    char payload[strlen(sections[2]) - 1];
    memset(payload, 0, strlen(sections[2]) - 1);
    strncpy(payload, sections[2] + 1, strlen(sections[2]) - 2);

    if (is_in_subnet(data, sections[1])) {
        inet_pton(AF_INET, sections[1], &fakeIp);
        if (!findMapping(sections[1], data, 0)) {
            printf("No ARP entry found\n");
            return 0;
        }
        port = atoi(findMapping(sections[1], data, 0));
    } else {
        // If not in subnet, check if gateway has been assigned a ll address
        // (a.k.a a port), if so, proceed with that as the receiver, but final
        // dst. IP still unchanged
        if (!data->gw) {
            printf("No gateway found\n");
            return 0;
        }
        inet_pton(AF_INET, sections[1], &fakeIp);
        if (!findMapping(data->gw, data, 0)) {
            printf("No ARP entry found\n");
            return 0;
        }
        port = atoi(findMapping(data->gw, data, 0));
    }

    // Instantiate sockaddr_in data. IPv4, port from ARP mappings and localhost
    memset(&receiverData, 0, sizeof(receiverData));
    receiverData.sin_family = AF_INET;
    receiverData.sin_port = htons(port);
    receiverData.sin_addr.s_addr = 0;

    IpPack* newPack = (IpPack*)malloc(sizeof(IpPack));
    memset(newPack, 0, sizeof(IpPack));
    construct_payload(data, payload, receiverData, newPack, fakeIp);
    return 0;
}



void split_input(char* input, char** sections, int oneSpace) {
    int sectionCounter = 0;
    int indexInSection = 0;
    //int lenOfMsg = len; // The length of "msg "
    sections[0] = (char*)malloc(sizeof(char));
    sections[1] = (char*)malloc(sizeof(char));
    sections[2] = (char*)malloc(sizeof(char));
    memset(sections[0], 0, 1);
    memset(sections[1], 0, 1);
    memset(sections[2], 0, 1);
    for (long unsigned i = 0; i < strlen(input) - 1; i++) {
        //printf("%d %d\n", sectionCounter, indexInSection);
        sections[sectionCounter] = (char*)realloc(sections[sectionCounter], 
                sizeof(char) * (indexInSection + 2));
        sections[sectionCounter][indexInSection + 1] = 0;
        if (input[i] == ' ') {
            if (oneSpace == 0) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                oneSpace++;
                continue;
            }
            if (oneSpace && sectionCounter == 2) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                continue;
            }
            sections[sectionCounter][indexInSection] = 0;
            sectionCounter++;
            if (sectionCounter == 3) {
                sectionCounter--;
            }
            indexInSection = 0;
            continue;
        } else {
            //printf("sectionCounter: %d, indexInSection: %d, char: %c\n", 
            //        sectionCounter, indexInSection, input[i]);
            sections[sectionCounter][indexInSection] = input[i];
            indexInSection++;
        }
    }
    if (DEBUG) {
    printf("sections[0]: %s\n", sections[0]);
    printf("sections[1]: %s\n", sections[1]);
    printf("sections[2]: %s\n", sections[2]);
    }
}

void split_input_old(char* input, char** sections, int oneSpace, int len) {
    int sectionCounter = 0;
    int indexInSection = 0;
    int firstSpace = oneSpace;
    int lenOfMsg = len; // The length of "msg "
    sections[0] = (char*)malloc(sizeof(char) * lenOfMsg);
    sections[1] = (char*)malloc(sizeof(char) * LONGEST_IP_WITH_NULL);
    sections[2] = (char*)malloc(sizeof(char) * BUFF);
    memset(sections[0], 0, lenOfMsg);
    memset(sections[1], 0, LONGEST_IP_WITH_NULL);
    memset(sections[2], 0, BUFF);
    for (long unsigned i = 0; i < strlen(input) - 1; i++) {
        //printf("%d %d\n", sectionCounter, indexInSection);
        if (input[i] == ' ') {
            if (firstSpace == 0) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                firstSpace++;
                continue;
            }
            if (firstSpace && sectionCounter == 2) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                continue;
            }
            sectionCounter++;
            if (sectionCounter == 3) {
                sectionCounter--;
            }
            indexInSection = 0;
            continue;
        } else {
            //printf("sectionCounter: %d, indexInSection: %d, char: %c\n", 
            //        sectionCounter, indexInSection, input[i]);
            sections[sectionCounter][indexInSection] = input[i];
            indexInSection++;
        }
    }
}


int is_in_subnet(struct Data* data, char* ip) {
    int debug = 0;
    uint32_t hostIP;
    uint32_t givenIP;
    int mask;
    char ipString[LONGEST_IP_PLUS_NULL];
    memset(ipString, 0, LONGEST_IP_PLUS_NULL);
    strcpy(ipString, ip);
    mask = (int)pow(2,32) - ((int)pow(2, 32 - (data->cidrLen + 1)) - 1);
    inet_pton(AF_INET, data->hostIP, &hostIP);
    inet_pton(AF_INET, ipString, &givenIP);
    givenIP = htonl(givenIP);
    hostIP = htonl(hostIP);
    if (debug) {
        printf("mask: %d\n", mask);
        printf("hostIP %u, givenIP %u\n", hostIP, givenIP);
        printf("hostIP %s, givenIP %s\n", data->hostIP, ipString);
    }

    if ((hostIP & mask) == (givenIP & mask)) {
        if (debug)
            printf("In same subnet\n");
        return 1;
    }
    if (debug)
        printf("Not in same subnet\n");

    return 0;
}

