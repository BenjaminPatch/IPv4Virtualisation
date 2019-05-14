CFLAGS = -Wall -pedantic

all: main.o process.o arp.o assign3

clean:
	rm *.o assign3

main.o: main.c
	gcc -c -g $(CFLAGS) main.c -o main.o

process.o: process.c
	gcc -c -g $(CFLAGS) process.c -o process.o

arp.o: arp.c
	gcc -c -g $(CFLAGS) arp.c -o arp.o

assign3: main.o process.o
	gcc $(CFLAGS) main.o arp.o process.o -o assign3

