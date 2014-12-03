#Makefile
CC = g++
INCLUDE = /usr/lib
LIBS = -pthread
OBJS = 
CFLAGS = -std=c++11

all: clean message threadpool processperprotocol

message:
	$(CC) -o message message.h $(CFLAGS) $(LIBS)
threadpool:
	$(CC) -o threadpool threadpool.h $(CFLAGS) $(LIBS)
processperprotocol:
	$(CC) -o processperprotocol processperprotocol.cpp $(CFLAGS) $(LIBS)

clean:
	rm -f driver1 driver2 driver3 message threadpool processperprotocol
