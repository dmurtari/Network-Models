#include "processperprotocol.h"

#define DEBUG 0

using namespace std;

ProcessPerProtocol::ProcessPerProtocol() {

  if(DEBUG) { cout << "Creating ThreadPool" << endl; }
  protocol_threads = new ThreadPool(17);
  
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
  protocol_threads->dispatch_thread(tcp_send, (void*) this);
  protocol_threads->dispatch_thread(udp_send, (void*) this);
  protocol_threads->dispatch_thread(ip_send, (void*) this);
  protocol_threads->dispatch_thread(ethernet_send, (void*) this);
  protocol_threads->dispatch_thread(receive_message, (void*) this);
  protocol_threads->dispatch_thread(ethernet_receive, (void*) this);
  protocol_threads->dispatch_thread(ip_receive, (void*) this);
  protocol_threads->dispatch_thread(tcp_receive, (void*) this);
  protocol_threads->dispatch_thread(udp_receive, (void*) this);
  protocol_threads->dispatch_thread(ftp_receive, (void*) this);
  protocol_threads->dispatch_thread(telnet_receive, (void*) this);
  protocol_threads->dispatch_thread(rdp_receive, (void*) this);
  protocol_threads->dispatch_thread(dns_receive, (void*) this);
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
    default:
      cout << "Invalid protocol. Can only send on FTP, TELNET, RDP, or DNS" << endl;
      break;
  }
}

void ProcessPerProtocol::ftp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from FTP Send" << endl; }
    pthread_mutex_unlock(&ppp->ftp_send_pipe.pipe_mutex);
    read(ppp->ftp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ftp_send_pipe.pipe_mutex);
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

void ProcessPerProtocol::ftp_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1){
    Message* received_message;
    char* received_message_buffer = new char[512];
    send_message* read_from_pipe = new send_message;

    if(DEBUG) { cout << "Locking and reading from FTP receive" << endl; }
    pthread_mutex_unlock(&ppp->ftp_receive_pipe.pipe_mutex);
    read(ppp->ftp_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ftp_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from FTP receive" << endl; }

    received_message = read_from_pipe->message;
    received_message->msgStripHdr(sizeof(ftp_header));
    received_message->msgFlat(received_message_buffer);

    cout << "Received message using FTP: " << received_message_buffer << endl;

    delete received_message_buffer;
  }
}

void ProcessPerProtocol::telnet_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from TELNET Send" << endl; }
    pthread_mutex_unlock(&ppp->telnet_send_pipe.pipe_mutex);
    read(ppp->telnet_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->telnet_send_pipe.pipe_mutex);
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

void ProcessPerProtocol::telnet_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1){
    Message* received_message;
    char* received_message_buffer = new char[512];
    send_message* read_from_pipe = new send_message;

    if(DEBUG) { cout << "Locking and reading from TELNET receive" << endl; }
    pthread_mutex_unlock(&ppp->telnet_receive_pipe.pipe_mutex);
    read(ppp->telnet_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->telnet_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from TELNET receive" << endl; }

    received_message = read_from_pipe->message;
    received_message->msgStripHdr(sizeof(telnet_header));
    received_message->msgFlat(received_message_buffer);

    cout << "Received message using telnet: " << received_message_buffer << endl;

    delete received_message_buffer;
  }
}

void ProcessPerProtocol::rdp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from RDP Send" << endl; }
    pthread_mutex_unlock(&ppp->rdp_send_pipe.pipe_mutex);
    read(ppp->rdp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->rdp_send_pipe.pipe_mutex);
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

    if(DEBUG) { cout << "Locking and writing message to UDP send" << endl; }
    pthread_mutex_lock(&ppp->udp_send_pipe.pipe_mutex);
    write(ppp->udp_send_pipe.pipes[1], (char *) &rdp_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->udp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to UDP Send" << endl; }
  }
}

void ProcessPerProtocol::rdp_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1){
    Message* received_message;
    char* received_message_buffer = new char[512];
    send_message* read_from_pipe = new send_message;

    if(DEBUG) { cout << "Locking and reading from RDP receive" << endl; }
    pthread_mutex_unlock(&ppp->rdp_receive_pipe.pipe_mutex);
    read(ppp->rdp_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->rdp_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from RDP receive" << endl; }

    received_message = read_from_pipe->message;
    received_message->msgStripHdr(sizeof(rdp_header));
    received_message->msgFlat(received_message_buffer);

    cout << "Received message using RDP: " << received_message_buffer << endl;

    delete received_message_buffer;
  }
}

void ProcessPerProtocol::dns_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from DNS Send" << endl; }
    pthread_mutex_unlock(&ppp->dns_send_pipe.pipe_mutex);
    read(ppp->dns_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->dns_send_pipe.pipe_mutex);
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

    if(DEBUG) { cout << "Locking and writing message to UDP send" << endl; }
    pthread_mutex_lock(&ppp->udp_send_pipe.pipe_mutex);
    write(ppp->udp_send_pipe.pipes[1], (char *) &dns_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->udp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to UDP Send" << endl; }
  }
}

void ProcessPerProtocol::dns_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1){
    Message* received_message;
    char* received_message_buffer = new char[512];
    send_message* read_from_pipe = new send_message;

    if(DEBUG) { cout << "Locking and reading from DNS receive" << endl; }
    pthread_mutex_unlock(&ppp->dns_receive_pipe.pipe_mutex);
    read(ppp->dns_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->dns_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from DNS receive" << endl; }

    received_message = read_from_pipe->message;
    received_message->msgStripHdr(sizeof(dns_header));
    received_message->msgFlat(received_message_buffer);

    cout << "Received message using DNS: " << received_message_buffer << endl;

    delete received_message_buffer;
  }
}

void ProcessPerProtocol::tcp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from TCP Send" << endl; }
    pthread_mutex_unlock(&ppp->tcp_send_pipe.pipe_mutex);
    read(ppp->tcp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->tcp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from TCP Send" << endl; }

    read_message = read_from_pipe->message;

    tcp_header* head = new tcp_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created TCP header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new TCP header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(tcp_header));

    if(DEBUG) {cout << "Creating message struct to send from TCP" << endl; }
    send_message tcp_send_message;
    tcp_send_message.protocol_id = TCP;
    tcp_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to IP send" << endl; }
    pthread_mutex_lock(&ppp->ip_send_pipe.pipe_mutex);
    write(ppp->ip_send_pipe.pipes[1], (char *) &tcp_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ip_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to IP Send" << endl; }
  }
}

void ProcessPerProtocol::tcp_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;
    int higher_level_protocol;

    if(DEBUG) { cout << "Locking and reading from TCP receive" << endl; }
    pthread_mutex_unlock(&ppp->tcp_receive_pipe.pipe_mutex);
    read(ppp->tcp_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->tcp_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from TCP receive" << endl; }

    read_message = read_from_pipe->message;
    tcp_header* stripped_header = (tcp_header *)read_message->msgStripHdr(sizeof(tcp_header));
    higher_level_protocol = stripped_header->higher_level_protocol;

    send_message tcp_message;
    tcp_message.protocol_id = TCP;
    tcp_message.message = read_message;

    if(DEBUG) { cout << "HLP from TCP is: " << higher_level_protocol << endl; }
    switch(higher_level_protocol) {
      case FTP:
        if(DEBUG) { cout << "Delivering message to FTP receive" << endl; }
        pthread_mutex_lock(&ppp->ftp_receive_pipe.pipe_mutex);
        write(ppp->ftp_receive_pipe.pipes[1], (char *) &tcp_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->ftp_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to FTP receive" << endl; }
      case TELNET:
        if(DEBUG) { cout << "Delivering message to TELNET receive" << endl; }
        pthread_mutex_lock(&ppp->telnet_receive_pipe.pipe_mutex);
        write(ppp->telnet_receive_pipe.pipes[1], (char *) &tcp_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->telnet_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to TELNET receive" << endl; }
      
    }
  }
}

void ProcessPerProtocol::udp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from UDP Send" << endl; }
    pthread_mutex_unlock(&ppp->udp_send_pipe.pipe_mutex);
    read(ppp->udp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->udp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from UDP Send" << endl; }

    read_message = read_from_pipe->message;

    udp_header* head = new udp_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created UDP header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new UDP header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(udp_header));

    if(DEBUG) {cout << "Creating message struct to send from UDP" << endl; }
    send_message udp_send_message;
    udp_send_message.protocol_id = UDP;
    udp_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to IP send" << endl; }
    pthread_mutex_lock(&ppp->ip_send_pipe.pipe_mutex);
    write(ppp->ip_send_pipe.pipes[1], (char *) &udp_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ip_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to IP Send" << endl; }
  }
}

void ProcessPerProtocol::udp_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;
    int higher_level_protocol;

    if(DEBUG) { cout << "Locking and reading from UDP receive" << endl; }
    pthread_mutex_unlock(&ppp->udp_receive_pipe.pipe_mutex);
    read(ppp->udp_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->udp_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from UDP receive" << endl; }

    read_message = read_from_pipe->message;
    udp_header* stripped_header = (udp_header *)read_message->msgStripHdr(sizeof(udp_header));
    higher_level_protocol = stripped_header->higher_level_protocol;

    send_message udp_message;
    udp_message.protocol_id = UDP;
    udp_message.message = read_message;

    if(DEBUG) { cout << "HLP from UDP is: " << higher_level_protocol << endl; }
    switch(higher_level_protocol) {
      case RDP:
        if(DEBUG) { cout << "Delivering message to RDP receive" << endl; }
        pthread_mutex_lock(&ppp->rdp_receive_pipe.pipe_mutex);
        write(ppp->rdp_receive_pipe.pipes[1], (char *) &udp_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->rdp_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to RDP receive" << endl; }
      case DNS:
        if(DEBUG) { cout << "Delivering message to DNS receive" << endl; }
        pthread_mutex_lock(&ppp->dns_receive_pipe.pipe_mutex);
        write(ppp->dns_receive_pipe.pipes[1], (char *) &udp_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->dns_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to DNS receive" << endl; }
      
    }
  }
}

void ProcessPerProtocol::ip_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;

    if(DEBUG) { cout << "Locking and reading from IP Send" << endl; }
    pthread_mutex_unlock(&ppp->ip_send_pipe.pipe_mutex);
    read(ppp->ip_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ip_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from IP Send" << endl; }

    read_message = read_from_pipe->message;

    ip_header* head = new ip_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created IP header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new IP header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(ip_header));

    if(DEBUG) {cout << "Creating message struct to send from IP" << endl; }
    send_message ip_send_message;
    ip_send_message.protocol_id = IP;
    ip_send_message.message = read_message;

    if(DEBUG) { cout << "Locking and writing message to ETHERNET send" << endl; }
    pthread_mutex_lock(&ppp->ethernet_send_pipe.pipe_mutex);
    write(ppp->ethernet_send_pipe.pipes[1], (char *) &ip_send_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ethernet_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done writing to ETHERNET Send" << endl; }
  }
}

void ProcessPerProtocol::ip_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;
    int higher_level_protocol;

    if(DEBUG) { cout << "Locking and reading from IP receive" << endl; }
    pthread_mutex_unlock(&ppp->ip_receive_pipe.pipe_mutex);
    read(ppp->ip_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ip_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from IP receive" << endl; }

    read_message = read_from_pipe->message;
    ip_header* stripped_header = (ip_header *)read_message->msgStripHdr(sizeof(ip_header));
    higher_level_protocol = stripped_header->higher_level_protocol;

    send_message ip_message;
    ip_message.protocol_id = IP;
    ip_message.message = read_message;

    if(DEBUG) { cout << "HLP from IP is: " << higher_level_protocol << endl; }
    switch(higher_level_protocol) {
      case TCP:
        if(DEBUG) { cout << "Delivering message to TCP receive" << endl; }
        pthread_mutex_lock(&ppp->tcp_receive_pipe.pipe_mutex);
        write(ppp->tcp_receive_pipe.pipes[1], (char *) &ip_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->tcp_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to TCP receive" << endl; }
      case UDP:
        if(DEBUG) { cout << "Delivering message to UDP receive" << endl; }
        pthread_mutex_lock(&ppp->udp_receive_pipe.pipe_mutex);
        write(ppp->udp_receive_pipe.pipes[1], (char *) &ip_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->udp_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to UDP receive" << endl; }
      
    }
  }
}

void ProcessPerProtocol::receive_message(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;
  struct sockaddr_in myaddr;
  struct sockaddr_in remaddr; 
  socklen_t addrlen = sizeof(remaddr);  
  int recvlen;     
  int fd;           
  char buf[512]; 

  /* create a UDP socket */

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("cannot create socket\n");
  }

  /* bind the socket to any valid IP address and a specific port */

  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(SERVICE_PORT);

  if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
  }

  /* now loop, receiving data and printing what we received */
  while(1) {
    printf("Waiting on port %d\n", SERVICE_PORT);
    recvlen = recvfrom(fd, buf, 512, 0, (struct sockaddr *)&remaddr, &addrlen);
    printf("Received %d bytes\n", recvlen);

    Message* received_message = new Message(buf, recvlen);

    send_message ethernet_message;
    ethernet_message.protocol_id = ETHERNET;
    ethernet_message.message = received_message;

    if(DEBUG) { cout << "Delivering message to ethernet receive" << endl; }
    pthread_mutex_lock(&ppp->ethernet_receive_pipe.pipe_mutex);
    write(ppp->ethernet_receive_pipe.pipes[1], (char *) &ethernet_message, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ethernet_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Delivered message to ethernet receive" << endl; }
  }
}

void ProcessPerProtocol::ethernet_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;
  struct sockaddr_in myaddr, remaddr;
  int udp_sock;
  char *server = "127.0.0.1"; 

  /* Create a socket */

  if ((udp_sock=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    printf("socket created\n");

  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(0);

  if (bind(udp_sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
  }       

  memset((char *) &remaddr, 0, sizeof(remaddr));
  remaddr.sin_family = AF_INET;
  remaddr.sin_port = htons(SERVICE_PORT);
  if (inet_aton(server, &remaddr.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
  }
  
  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;
    char message_buffer[512];

    if(DEBUG) { cout << "Locking and reading from ethernet Send" << endl; }
    pthread_mutex_unlock(&ppp->ethernet_send_pipe.pipe_mutex);
    read(ppp->ethernet_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ethernet_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from ethernet Send" << endl; }

    read_message = read_from_pipe->message;

    ethernet_header* head = new ethernet_header;
    head->higher_level_protocol = read_from_pipe->protocol_id;
    head->message_length = read_message->msgLen();

    if(DEBUG){
      cout << "Created ethernet header with HLP " << head->higher_level_protocol;
      cout << " and message_length " << head->message_length << endl;
    }

    if(DEBUG) { cout << "Adding new ethernet header to message" << endl; }
    read_message->msgAddHdr((char*) head, sizeof(ethernet_header));

    cout << "Sending over ethernet" << endl;
    memset(&message_buffer, 0, sizeof(message_buffer));

    read_message->msgFlat(message_buffer);
    if(sendto(udp_sock, message_buffer, read_message->msgLen(), 0, 
              (struct sockaddr *)&remaddr, sizeof(remaddr)) < 0)
      printf("Error with sendto %s\n", strerror(errno));
  }
}

void ProcessPerProtocol::ethernet_receive(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;
    Message* read_message;
    int higher_level_protocol;

    if(DEBUG) { cout << "Locking and reading from ethernet receive" << endl; }
    pthread_mutex_unlock(&ppp->ethernet_receive_pipe.pipe_mutex);
    read(ppp->ethernet_receive_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_lock(&ppp->ethernet_receive_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from etherner receive" << endl; }

    read_message = read_from_pipe->message;
    ethernet_header* stripped_header = (ethernet_header *)read_message->msgStripHdr(sizeof(ethernet_header));
    higher_level_protocol = stripped_header->higher_level_protocol;

    send_message ethernet_message;
    ethernet_message.protocol_id = ETHERNET;
    ethernet_message.message = read_message;

    if(DEBUG) { cout << "HLP from etherner is: " << higher_level_protocol << endl; }
    switch(higher_level_protocol) {
      case IP:
        if(DEBUG) { cout << "Delivering message to IP receive" << endl; }
        pthread_mutex_lock(&ppp->ip_receive_pipe.pipe_mutex);
        write(ppp->ip_receive_pipe.pipes[1], (char *) &ethernet_message, sizeof(send_message));
        pthread_mutex_unlock(&ppp->ip_receive_pipe.pipe_mutex);
        if(DEBUG) { cout << "Delivered message to IP receive" << endl; }
    }
  }
}
