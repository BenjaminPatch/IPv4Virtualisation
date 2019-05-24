CFLAGS = -Wall -pedantic -pthread

all: main.o packets.o process.o arp.o listen.o assign3

clean:
	rm *.o assign3

main.o: main.c
	gcc -c -g $(CFLAGS) main.c -o main.o

process.o: process.c
	gcc -c -g $(CFLAGS) process.c -o process.o -lm 

arp.o: arp.c
	gcc -c -g $(CFLAGS) arp.c -o arp.o

packets.o: packets.c
	gcc -c -g $(CFLAGS) packets.c -o packets.o

listen.o: listen.c
	gcc -c -g $(CFLAGS) listen.c -o listen.o

assign3: main.o process.o arp.o packets.o
	gcc $(CFLAGS) main.o arp.o packets.o listen.o process.o -o assign3 -lm

