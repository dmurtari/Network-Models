#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>

#include "threadpool.h"
#include "message.h"
#include "protocolid.h"
#include "protocol_application.h"

using namespace std;

class ProcessPerProtocol {
  public: 
    /* Constructor and destructor */
    ProcessPerProtocol();
    ~ProcessPerProtocol();

    /* Container struct to allow a mutex to be associated with a pipe. Send and
       receive of each protocol will have its own pipe */
    struct pipe_container {
      int pipes[2];
      pthread_mutex_t pipe_mutex;
    };

    /* Applications need some way to initiate commmunication */
    void application_send_msg(send_message message, int protocol_id);
    pipe_container ftp_send_pipe, telnet_send_pipe, rdp_send_pipe, dns_send_pipe;

  private:
    /* Thread pool to contain threads for each protocol */
    ThreadPool* protocol_threads;

    /* Each protocol needs a way to send and receive messages from its pipes */
    static void ftp_send(void* arg);
    static void ftp_receive(void* arg);
    static void telnet_send(void* arg);
    static void telnet_receive(void* arg);
    static void rdp_send(void* arg);
    static void rdp_receive(void* arg);
    static void dns_send(void* arg);
    static void dns_receive(void* arg);
    static void tcp_send(void* arg);
    static void tcp_receive(void* arg);
    static void udp_send(void* arg);
    static void udp_receive(void* arg);
    static void ip_send(void* arg);
    static void ip_receive(void* arg);
    static void ethernet_send(void* arg);
    static void ethernet_receive(void* arg);
    static void receive_message(void* arg);

    pipe_container ftp_receive_pipe, telnet_receive_pipe, rdp_receive_pipe,
                   dns_receive_pipe;
    pipe_container tcp_send_pipe, tcp_receive_pipe;
    pipe_container udp_send_pipe, udp_receive_pipe;
    pipe_container ip_send_pipe, ip_receive_pipe;
    pipe_container ethernet_send_pipe, ethernet_receive_pipe;

    /* Header structs for each of the headers needing to be supported in PPP */
    struct ftp_header {
      int higher_level_protocol;
      char other_info[8];
      int message_length;           
    };

    struct telnet_header {
      int higher_level_protocol;
      char other_info[8];
      int message_length;           
    };

    struct rdp_header {
      int higher_level_protocol;
      char other_info[12];
      int message_length;           
    };

    struct dns_header {
      int higher_level_protocol;
      char other_info[8];
      int message_length;           
    };

    struct tcp_header {
      int higher_level_protocol;
      char other_info[4];
      int message_length;           
    };

    struct udp_header {
      int higher_level_protocol;
      char other_info[4];
      int message_length;           
    };

    struct ip_header {
      int higher_level_protocol;
      char other_info[12];
      int message_length;           
    };

    struct ethernet_header {
      int higher_level_protocol;
      char other_info[8];
      int message_length;           
    };
};