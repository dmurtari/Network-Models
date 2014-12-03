CC=g++
FLAGS=-g -std=gnu++11 -Wall -Wextra
LIBS=-lpthread

all: threadpool.o eventscheduler.o processperprotocol

processperprotocol: processperprotocol.cpp threadpool.o message.o
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

threadpool.o: threadpool.cpp
	$(CC) $(FLAGS) -o $@ -c $^

eventscheduler.o: eventscheduler.cpp
	$(CC) $(FLAGS) -o $@ -c $^

message.o: message.cpp
	$(CC) $(FLAGS) -o $@ -c $^

clean:
	rm -f threadpool.o eventscheduler.o processperprotocol
