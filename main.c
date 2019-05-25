#include "main.h"
sem_t outputLock;

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "argc != 3\n");
        exit(1);
    }
    struct Data* data;
    struct Dict* arpMappings;
    pthread_t listenThread;

    /* Initialise data, which holds info about whole network */
    data = (struct Data*)malloc(sizeof(struct Data));
    memset(data, 0, sizeof(struct Data));
    arpMappings = (struct Dict*)malloc(sizeof(struct Dict));
    arpMappings->val = NULL;
    arpMappings->next = NULL;
    data->arpMappings = arpMappings;
    data->MTU = 1500;
    data->idNum = 0;
    parse_ipAddr(data, argv[1]);
    if (!(data->hostLL = atoi(argv[2])))
        fprintf(stderr, "llAddr Not a Number\n");

    sem_init(&outputLock, 0, 1);
    data->outputLock = &outputLock;
    pthread_create(&listenThread, NULL, listen_for_message, (void*)data);

    run_program(data);
    return 0;
}

void run_program(struct Data* data) {
    char input[BUFFER];
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
        } else if (!strncmp(input, "exit", 4)) {
            exit(0);
        } else if (!strncmp(input, "mtu set", 7)) {
            if (mtu_set(input, data)) fprintf(stderr, "error in mtu_set\n");
        } else if (!strncmp(input, "mtu get", 7)) {
            if (mtu_get(data, 0)) fprintf(stderr, "error in arp_get\n");
        } else if (!strncmp(input, "msg", 3)) {
            if (send_msg(data, input)) fprintf(stderr, "error in send_msg\n");
        }
        sem_post(data->outputLock);
    }
}
