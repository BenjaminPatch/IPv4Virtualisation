#include "arp.h"

void append_to_dict(struct Dict* dict, struct Arp* mapping) {
    struct Dict* current = dict;
    if (current->val == NULL) {
        current->val = (struct Arp*)mapping;
        current->next = NULL;
        return;
    }

    // check if mapping already exists, if so, clear it and make new
    if (!strcmp(((struct Arp*)current->val)->ipAddr, mapping->ipAddr)) {
        memset(((struct Arp*)current->val)->llAddr, 0, LARGEST_PORT_LEN);
        strcpy(((struct Arp*)current->val)->llAddr, mapping->llAddr);
        return;
    }
    while (current->next != NULL) {
        if (!strcmp(((struct Arp*)current->val)->ipAddr, mapping->ipAddr)) {
            memset(((struct Arp*)current->val)->llAddr, 0, LARGEST_PORT_LEN);
            strcpy(((struct Arp*)current->val)->llAddr, mapping->llAddr);
            return;
        }
        current = current->next;
    }

    // Append new var
    current->next = (struct Dict*)malloc(sizeof(struct Dict));
    current->next->val = (struct Arp*)mapping;
    current->next->next = NULL;
}


void display_dict_entries(struct Dict* dict) {
    struct Dict* current = dict;

    while (current != NULL) {
        printf("ipAddr: %s.   llAddr: %s\n", 
                ((struct Arp*)current->val)->ipAddr, 
                ((struct Arp*)current->val)->llAddr);
        current = current->next;
    }

}


char* findMapping(char* ip, struct Data* data, int cliMode) {
    struct Dict* current = data->arpMappings;
    strtok(ip, "\n");

    while (current != NULL && current->val != NULL) {
        if (!strcmp(((struct Arp*)current->val)->ipAddr, ip)) {
            if (cliMode)
                printf("%s\n", ((struct Arp*)current->val)->llAddr);
            return ((struct Arp*)current->val)->llAddr;
        }
        current = current->next;
    }
    if (cliMode)
        printf("None\n");
    sem_wait(data->outputLock);
    fflush(stdout);
    return NULL;

}

