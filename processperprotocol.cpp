#include "processperprotocol.h"

#define DEBUG 1

using namespace std;

ProcessPerProtocol::ProcessPerProtocol() {

  if(DEBUG) { cout << "Creating ThreadPool" << endl; }
  protocol_threads = new ThreadPool(16);
  
  /* Create pipes */
  if(DEBUG) { cout << "Creating Pipes" << endl; }
  pipe(ftp_send_pipe.pipes); pipe(ftp_receive_pipe.pipes);
  pipe(telnet_send_pipe.pipes); pipe(telnet_receive_pipe.pipes);
  pipe(rdp_send_pipe.pipes); pipe(rdp_receive_pipe.pipes);
  pipe(dns_send_pipe.pipes); pipe(dns_receive_pipe.pipes);
  pipe(tcp_send_pipe.pipes); pipe(tcp_receive_pipe.pipes);
  pipe(ip_send_pipe.pipes);  pipe(ip_receive_pipe.pipes);
  pipe(ethernet_send_pipe.pipes); pipe(ethernet_receive_pipe.pipes);

  /* Initialize mutexes to control access to pipes */
  if(DEBUG) { cout << "Initializing Mutexes" << endl; }
  pthread_mutex_init(&ftp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ftp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&telnet_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&telnet_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&rdp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&rdp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&dns_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&dns_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&tcp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&tcp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&udp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&udp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ip_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ip_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ethernet_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ethernet_receive_pipe.pipe_mutex, NULL);

  if(DEBUG) { cout << "Dispatching Threads" << endl; }
  protocol_threads->dispatch_thread(ftp_send, (void*) this);
  protocol_threads->dispatch_thread(telnet_send, (void*) this);
  protocol_threads->dispatch_thread(rdp_send, (void*) this);
  protocol_threads->dispatch_thread(dns_send, (void*) this);
}

void ProcessPerProtocol::application_send_msg(send_message message, 
                                              int protocol_id) {
  switch(protocol_id){
    case FTP:
      if(DEBUG) { cout << "Locking and writing to FTP Send" << endl; }
      pthread_mutex_lock(&ftp_send_pipe.pipe_mutex);
      write(ftp_send_pipe.pipes[1], (char *) &message, sizeof(send_message));
      pthread_mutex_unlock(&ftp_send_pipe.pipe_mutex);
      if(DEBUG) { cout << "Done writing to FTP Send" << endl; }
      break;
    case TELNET:
      if(DEBUG) { cout << "Locking and writing to TELNET Send" << endl; }
      pthread_mutex_lock(&telnet_send_pipe.pipe_mutex);
      write(telnet_send_pipe.pipes[1], (char *) &message, sizeof(send_message));
      pthread_mutex_unlock(&telnet_send_pipe.pipe_mutex);
      if(DEBUG) { cout << "Done writing to TELNET Send" << endl; }
      break;
    case RDP:
      if(DEBUG) { cout << "Locking and writing to RDP Send" << endl; }
      pthread_mutex_lock(&rdp_send_pipe.pipe_mutex);
      write(rdp_send_pipe.pipes[1], (char *) &message, sizeof(send_message));
      pthread_mutex_unlock(&rdp_send_pipe.pipe_mutex);
      if(DEBUG) { cout << "Done writing to RDP Send" << endl; }
      break;
    case DNS:
      if(DEBUG) { cout << "Locking and writing to DNS Send" << endl; }
      pthread_mutex_lock(&dns_send_pipe.pipe_mutex);
      write(dns_send_pipe.pipes[1], (char *) &message, sizeof(send_message));
      pthread_mutex_unlock(&dns_send_pipe.pipe_mutex);
      if(DEBUG) { cout << "Done writing to DNS Send" << endl; }
      break;
  }
}

void ProcessPerProtocol::ftp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from FTP Send" << endl; }
    pthread_mutex_lock(&ppp->ftp_send_pipe.pipe_mutex);
    read(ppp->ftp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ftp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from FTP Send" << endl; }

    read_message = read_from_pipe->message;

    ftp_header* head = new ftp_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created FTP header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new FTP header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(ftp_header));

    if(DEBUG) {cout << "Creating message struct to send from FTP" << endl; }
    send_message ftp_send_message;
    ftp_send_message.protocol_id = FTP;
    ftp_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to TCP send" << endl; }
    pthread_mutex_lock(&ppp->tcp_send_pipe.pipe_mutex);
    write(ppp->tcp_send_pipe.pipes[1], (char *) &ftp_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->tcp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to TCP Send" << endl; }
  }
}

void ProcessPerProtocol::telnet_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from TELNET Send" << endl; }
    pthread_mutex_lock(&ppp->telnet_send_pipe.pipe_mutex);
    read(ppp->telnet_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_unlock(&ppp->telnet_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from TELNET Send" << endl; }

    read_message = read_from_pipe->message;

    telnet_header* head = new telnet_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created telnet header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new TELNET header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(telnet_header));

    if(DEBUG) {cout << "Creating message struct to send from TELNET" << endl; }
    send_message telnet_send_message;
    telnet_send_message.protocol_id = TELNET;
    telnet_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to TCP send" << endl; }
    pthread_mutex_lock(&ppp->tcp_send_pipe.pipe_mutex);
    write(ppp->tcp_send_pipe.pipes[1], (char *) &telnet_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->tcp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to TCP Send" << endl; }
  }
}

void ProcessPerProtocol::rdp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from RDP Send" << endl; }
    pthread_mutex_lock(&ppp->rdp_send_pipe.pipe_mutex);
    read(ppp->rdp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_unlock(&ppp->rdp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from RDP Send" << endl; }

    read_message = read_from_pipe->message;

    rdp_header* head = new rdp_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created RDP header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new RDP header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(rdp_header));

    if(DEBUG) {cout << "Creating message struct to send from RDP" << endl; }
    send_message rdp_send_message;
    rdp_send_message.protocol_id = RDP;
    rdp_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to TCP send" << endl; }
    pthread_mutex_lock(&ppp->tcp_send_pipe.pipe_mutex);
    write(ppp->tcp_send_pipe.pipes[1], (char *) &rdp_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->tcp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to TCP Send" << endl; }
  }
}

void ProcessPerProtocol::dns_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from DNS Send" << endl; }
    pthread_mutex_lock(&ppp->dns_send_pipe.pipe_mutex);
    read(ppp->dns_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_unlock(&ppp->dns_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from DNS Send" << endl; }

    read_message = read_from_pipe->message;

    dns_header* head = new dns_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created DNS header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new DNS header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(dns_header));

    if(DEBUG) {cout << "Creating message struct to send from DNS" << endl; }
    send_message dns_send_message;
    dns_send_message.protocol_id = DNS;
    dns_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to TCP send" << endl; }
    pthread_mutex_lock(&ppp->tcp_send_pipe.pipe_mutex);
    write(ppp->tcp_send_pipe.pipes[1], (char *) &dns_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->tcp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to TCP Send" << endl; }
  }
}