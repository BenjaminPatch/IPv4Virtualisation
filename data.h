#ifndef DATA_H
#define DATA_H

struct Data {
    char* gw;
    char* arpIP;
    char* arpLL;
    int MTU;
    struct Dict* arpMappings;
    int idNum;
};

#endif 
