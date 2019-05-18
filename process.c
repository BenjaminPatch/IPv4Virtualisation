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
        printf("None\n");
    } else {
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
    display_dict_entries(data->arpMappings);

    return 0;
}

int arp_get(char* input, struct Data* data) {
    if (data->arpMappings->val == NULL) {
        printf("None\n");
    } else {
        const char s[2] = " ";
        char* tok = strtok(input, s);
        tok = strtok(0, s);
        tok = strtok(0, s);
        int cliMode = 1;
        findMapping(tok, data->arpMappings, cliMode);
    }

    return 0;
}


int mtu_set(char* input, struct Data* data) {
    strtok(input, "\n");
    data->MTU = atoi(input + 8);
    return 0;
}


int mtu_get(struct Data* data) {
    printf("%d\n", data->MTU);
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
    printf("hey\n");
    int sockfd, port;
    struct sockaddr_in receiverData;
    int lenOfMsg = 4; // The length of "msg "
    char** sections = (char**)malloc(sizeof(char*) * 3);
    sections[0] = (char*)malloc(sizeof(char) * lenOfMsg);
    sections[1] = (char*)malloc(sizeof(char) * LONGEST_IP_WITH_NULL);
    sections[2] = (char*)malloc(sizeof(char) * BUFF);
    memset(sections[0], 0, lenOfMsg);
    memset(sections[1], 0, LONGEST_IP_WITH_NULL);
    memset(sections[2], 0, BUFF);
    printf("0\n");
    split_input(input, sections, 1);
    printf("0.5\n");

    //char* payload = (char*)malloc(sizeof(char) * strlen(sections[2]) - 1);
    char payload[strlen(sections[2] - 1)];
    memset(payload, 0, strlen(sections[2]) - 1);
    printf("1\n");
    strncpy(payload, sections[2] + 1, strlen(sections[2]) - 2);
    printf("2\n");
    //strip_quotes(sections[2], payload);

    if ((findMapping(sections[1], data->arpMappings, 0)) == NULL) {
        printf("No ARP entry found\n");
        fflush(stdout);
        return 0;
    }
    port = atoi(findMapping(sections[1], data->arpMappings, 0));
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
    }
    memset(&receiverData, 0, sizeof(receiverData));

    receiverData.sin_family = AF_INET;
    receiverData.sin_port = htons(port);
    inet_pton(AF_INET, sections[1], &(receiverData.sin_addr));
    printf("%u\n", receiverData.sin_addr.s_addr);

    return 0;
}

void strip_quotes(char* section, char* payload) {
    char temp[strlen(section)];
    memset(temp, 0, strlen(section));
    printf("temP: %s\n", temp);

}

void split_input(char* input, char** sections, int oneSpace) {
    int sectionCounter = 0;
    int indexInSection = 0;
    int firstSpace = oneSpace;
    for (long unsigned i = 0; i < strlen(input) - 1; i++) {
        if (input[i] == ' ') {
            if (firstSpace == 0) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                firstSpace++;
                continue;
            }
            sectionCounter++;
            if (sectionCounter == 3) {
                sectionCounter--;
            }
            indexInSection = 0;
            continue;
        } else {
            printf("sectionCounter: %d, indexInSection: %d\n", sectionCounter, indexInSection);
            sections[sectionCounter][indexInSection] = input[i];
            indexInSection++;
        }
    }
}

