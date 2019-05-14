#include "main.h"

int main(int argc, char** argv) {
    char input[BUFFER];
    if (argc != 3) {
        fprintf(stderr, "argc != 3\n");
        exit(1);
    }
    struct Data* data = (struct Data*)malloc(sizeof(struct Data));
    memset(data, 0, sizeof(struct Data));
    struct Dict* arpMappings = (struct Dict*)malloc(sizeof(struct Dict));
    arpMappings->val = NULL;
    arpMappings->next = NULL;
    data->arpMappings = arpMappings;
    data->MTU = 1500;
    data->idNum = 0;
    parse_ipAddr(data, argv[1]);

    while (1) {
        printf("> ");
        if (!fgets(input, BUFFER, stdin)) {
            fprintf(stderr, "fgets\n");
            exit(1);
        }
        if (!strncmp(input, "gw set", 6)) {
            if (gw_set(input, data)) fprintf(stderr, "error in gw_set\n");
        } else if (!strncmp(input, "gw get", 6)) {
            if (gw_get(data)) fprintf(stderr, "error in gw_get\n");
        } else if (!strncmp(input, "arp set", 7)) {
            if (arp_set(input, data)) fprintf(stderr, "error in arp_set\n");
        } else if (!strncmp(input, "arp get", 7)) {
            if (arp_get(input, data)) fprintf(stderr, "error in arp_get\n");
        } else if (!strcmp(input, "exit")) {
            exit(0);
        } else if (!strncmp(input, "mtu set", 7)) {
            if (mtu_set(input, data)) fprintf(stderr, "error in mtu_set\n");
        } else if (!strncmp(input, "mtu get", 7)) {
            if (mtu_get(data)) fprintf(stderr, "error in arp_get\n");
        }
    }
}
