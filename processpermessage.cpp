#include "processpermessage.h"

#define DEBUG 0

ProcessPerMessage::ProcessPerMessage() {
  if(DEBUG) { cout << "Contructing ProcessPerMessage Object" << endl; }
  message_threads = new ThreadPool(10);
}

ProcessPerMessage::~ProcessPerMessage() {

}

void ProcessPerMessage::application_send_msg(Message* message, int protocol_id) {
  switch(protocol_id) {
    case FTP:
      if(DEBUG) { cout << "Received FTP send request, sending to FTP" << endl; }
      ftp_send(message, APPLICATION);
      break;
    case TELNET:
      if(DEBUG) { cout << "Received telnet send request, sending to telnet" << endl; }
      telnet_send(message, APPLICATION);
      break;
    case RDP:
      if(DEBUG) { cout << "Received RDP send request, sending to RDP" << endl; }
      rdp_send(message, APPLICATION);
      break;
    case DNS:
      if(DEBUG) { cout << "Received DNS send request, sending to DNS" << endl; }
      dns_send(message, APPLICATION);
      break;
    default:
      cout << "Invalid protocol to send to" << endl;
  }
}

void ProcessPerMessage::ftp_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received FTP message, attaching FTP header" << endl; }
  
  ftp_header* header = new ftp_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();
  message->msgAddHdr((char*) header, sizeof(ftp_header));

  if(DEBUG) { cout << "Done attaching FTP header, sending to TCP" << endl; }
  tcp_send(message, FTP);
}

void ProcessPerMessage::ftp_receive(Message* message) {
    char received[1024];

    message->msgStripHdr(sizeof(ftp_header));
    message->msgFlat(received);
    cout << "Received message on FTP: " << received << endl;
}

void ProcessPerMessage::telnet_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received telnet message, attaching telnet header" << endl; }
  
  telnet_header* header = new telnet_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(telnet_header));

  if(DEBUG) { cout << "Done attaching telnet header, sending to TCP" << endl; }
  tcp_send(message, TELNET);
}

void ProcessPerMessage::telnet_receive(Message* message) {
    char received[1024];

    message->msgStripHdr(sizeof(telnet_header));
    message->msgFlat(received);
    cout << "Received message on telnet: " << received << endl;
}

void ProcessPerMessage::rdp_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received RDP message, attaching RDP header" << endl; }
  
  rdp_header* header = new rdp_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(rdp_header));

  if(DEBUG) { cout << "Done attaching RDP header, sending to UDP" << endl; }
  udp_send(message, RDP);
}

void ProcessPerMessage::rdp_receive(Message* message) {
    char received[1024];

    message->msgStripHdr(sizeof(rdp_header));
    message->msgFlat(received);
    cout << "Received message on RDP: " << received << endl;
}

void ProcessPerMessage::dns_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received DNS message, attaching DNS header" << endl; }
  
  dns_header* header = new dns_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(dns_header));

  if(DEBUG) { cout << "Done attaching DNS header, sending to UDP" << endl; }
  udp_send(message, DNS);
}

void ProcessPerMessage::dns_receive(Message* message) {
    char received[1024];

    message->msgStripHdr(sizeof(dns_header));
    message->msgFlat(received);
    cout << "Received message on DNS: " << received << endl;
}

void ProcessPerMessage::tcp_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received TCP message, attaching TCP header" << endl; }
  
  tcp_header* header = new tcp_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(tcp_header));

  if(DEBUG) { cout << "Done attaching TCP header, sending to IP" << endl; }
  ip_send(message, TCP);
}

void ProcessPerMessage::tcp_receive(Message* message) {
  int higher_level_protocol;

  if(DEBUG) { cout << "Received TCP message, stripping TCP header" << endl; }

  tcp_header* stripped_header = (tcp_header *)message->msgStripHdr(sizeof(tcp_header));
  higher_level_protocol = stripped_header->higher_level_protocol;

  if(DEBUG) { cout << "Stripped header with hlp: " << higher_level_protocol << endl; }
  switch(higher_level_protocol){
    case FTP:
      ftp_receive(message);
      break;
    case TELNET:
      telnet_receive(message);
      break;
    default:
      cout << "Higher level prococol " << higher_level_protocol << " is invalid" << endl;
  }
}

void ProcessPerMessage::udp_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received UDP message, attaching UDP header" << endl; }
  
  udp_header* header = new udp_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(udp_header));

  if(DEBUG) { cout << "Done attaching UDP header, sending to IP" << endl; }
  ip_send(message, UDP);
}

void ProcessPerMessage::udp_receive(Message* message) {
  int higher_level_protocol;

  if(DEBUG) { cout << "Received UDP message, stripping UDP header" << endl; }

  udp_header* stripped_header = (udp_header *)message->msgStripHdr(sizeof(udp_header));
  higher_level_protocol = stripped_header->higher_level_protocol;

  if(DEBUG) { cout << "Stripped header with hlp: " << higher_level_protocol << endl; }
  switch(higher_level_protocol){
    case RDP:
      rdp_receive(message);
      break;
    case DNS:
      dns_receive(message);
      break;
    default:
      cout << "Higher level prococol " << higher_level_protocol << " is invalid" << endl;
  }
}

void ProcessPerMessage::ip_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received IP message, attaching IP header" << endl; }
  
  ip_header* header = new ip_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(ip_header));

  if(DEBUG) { cout << "Done attaching IP header, sending to ethernet" << endl; }
  ethernet_send(message, IP);
}

void ProcessPerMessage::ip_receive(Message* message) {
  int higher_level_protocol;

  if(DEBUG) { cout << "Received IP message, stripping IP header" << endl; }

  ip_header* stripped_header = (ip_header *)message->msgStripHdr(sizeof(ip_header));
  higher_level_protocol = stripped_header->higher_level_protocol;

  if(DEBUG) { cout << "Stripped header with hlp: " << higher_level_protocol << endl; }
  switch(higher_level_protocol){
    case TCP:
      tcp_receive(message);
      break;
    case UDP:
      udp_receive(message);
      break;
    default:
      cout << "Higher level prococol " << higher_level_protocol << " is invalid" << endl;
  }
}

void ProcessPerMessage::ethernet_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received ethernet message, attaching ethernet header" << endl; }
  
  ethernet_header* header = new ethernet_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(ethernet_header));

  if(DEBUG) { cout << "Done attaching ethernet header: " << header->higher_level_protocol << endl; }
  ethernet_receive(message);
}

void ProcessPerMessage::ethernet_receive(Message* message) {
  int higher_level_protocol;

  if(DEBUG) { cout << "Received ethernet message, stripping ethernet header" << endl; }

  ethernet_header* stripped_header = (ethernet_header *)message->msgStripHdr(sizeof(ethernet_header));
  higher_level_protocol = stripped_header->higher_level_protocol;

  if(DEBUG) { cout << "Stripped header with hlp: " << higher_level_protocol << endl; }
  switch(higher_level_protocol){
    case IP:
      ip_receive(message);
      break;
    default:
      cout << "Higher level prococol " << higher_level_protocol << " is invalid" << endl;
  }
}