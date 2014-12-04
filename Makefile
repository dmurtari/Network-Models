CC=g++
FLAGS=-g -std=gnu++11 -Wall -Wextra
LIBS=-lpthread


all: threadpool.o eventscheduler.o processperprotocol.o processpermessage protocol_application 

protocol_application: processperprotocol.o message.o threadpool.o protocol_application.cpp
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

processpermessage: processpermessage.cpp message.o threadpool.o
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

processperprotocol.o: processperprotocol.cpp
	$(CC) $(FLAGS) -o $@ -c $^

threadpool.o: threadpool.cpp
	$(CC) $(FLAGS) -o $@ -c $^

eventscheduler.o: eventscheduler.cpp
	$(CC) $(FLAGS) -o $@ -c $^

message.o: message.cpp
	$(CC) $(FLAGS) -o $@ -c $^

clean:
	rm -f threadpool.o eventscheduler.o processperprotocol.o protocol_application

