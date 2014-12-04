CC=g++
FLAGS=-g -std=gnu++11 -Wall -Wextra
LIBS=-lpthread


all: message_application protocol_application 

protocol_application: processperprotocol.o message.o threadpool.o protocol_application.cpp
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

message_application: processpermessage.o message.o threadpool.o message_application.cpp
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

processpermessage.o: processpermessage.cpp 
	$(CC) $(FLAGS) -o $@ -c $^

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

