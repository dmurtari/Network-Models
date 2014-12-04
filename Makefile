CC=g++
FLAGS=-g -std=gnu++11 -Wall -Wextra
LIBS=-lpthread


all: threadpool.o eventscheduler.o processperprotocol.o udp_socket.o protocol_application

protocol_application: processperprotocol.o message.o threadpool.o udp_socket.o protocol_application.cpp
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

udp_socket.o: udp_socket.cpp
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

