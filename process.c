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
    char** sections = (char**)malloc(sizeof(char*) * 2);

    // Split input so we can easily access ipAddr and llAddr
    split_input(input, sections);

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
        findMapping(tok, data->arpMappings);
    }

    return 0;
}

void split_input(char* input, char** sections) {
    int sectionCounter = 0;
    int indexInSection = 0;
    int firstSpace = 0;
    int lenOfArpSet = 8; // Length of string "arp set "
    sections[0] = (char*)malloc(sizeof(char*) * lenOfArpSet);
    sections[1] = (char*)malloc(sizeof(char*) * LONGEST_IP_PLUS_NULL);
    sections[2] = (char*)malloc(sizeof(char*) * LARGEST_PORT_LEN);
    memset(sections[0], 0, lenOfArpSet);
    memset(sections[1], 0, LONGEST_IP_PLUS_NULL);
    memset(sections[2], 0, LARGEST_PORT_LEN);
    for (long unsigned i = 0; i < strlen(input) - 1; i++) {
        if (input[i] == ' ') {
            if (firstSpace == 0) {
                sections[sectionCounter][indexInSection] = input[i];
                indexInSection++;
                firstSpace++;
                continue;
            }
            sectionCounter++;
            indexInSection = 0;
            continue;
        } else {
            //printf("sectionCounter: %d. indexInSection: %d\n", 
            //        sectionCounter, indexInSection);
            sections[sectionCounter][indexInSection] = input[i];
            indexInSection++;
        }
    }
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
    char* ipAddr = (char*)malloc(sizeof(char) * LONGEST_IP_PLUS_NULL);
    memset(ipAddr, 0, LONGEST_IP_PLUS_NULL);
    printf("argv1: %s\n", argv1);
    for (long unsigned i = 0; i < strlen(argv1) - 1; i++) {
        printf("%c", argv1[i]);
    }
}
