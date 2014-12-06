#ifndef _PROCESSPERMESSAGE_
#define _PROCESSPERMESSAGE_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>

#include "threadpool.h"
#include "message.h"
#include "protocolid.h"

class ProcessPerMessage {
  public:
    ProcessPerMessage(const char* input, const char* output);
    ~ProcessPerMessage();
    void application_send_msg(Message* message, int protocol_id);

  private:
    ThreadPool* message_threads;
    const char* input_port;
    const char* output_port;

    void ftp_send(Message* message, int higher_level_protocol);
    static void ftp_receive(Message* message);
    void telnet_send(Message* message, int higher_level_protocol);
    static void telnet_receive(Message* message);
    void rdp_send(Message* message, int higher_level_protocol);
    static void rdp_receive(Message* message);
    void dns_send(Message* message, int higher_level_protocol);
    static void dns_receive(Message* message);
    void tcp_send(Message* message, int higher_level_protocol);
    static void tcp_receive(Message* message);
    void udp_send(Message* message, int higher_level_protocol);
    static void udp_receive(Message* message);
    void ip_send(Message* message, int higher_level_protocol);
    static void ip_receive(Message* message);
    void ethernet_send(Message* message, int higher_level_protocol);
    static void ethernet_receive(void* arg);
    static void receive_message(void* arg);
};

#endif