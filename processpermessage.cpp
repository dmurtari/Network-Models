#include "processpermessage.h"

#define DEBUG 1

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

void ProcessPerMessage::telnet_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received telnet message, attaching telnet header" << endl; }
  
  telnet_header* header = new telnet_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(telnet_header));

  if(DEBUG) { cout << "Done attaching telnet header, sending to TCP" << endl; }
  tcp_send(message, TELNET);
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

void ProcessPerMessage::dns_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received DNS message, attaching DNS header" << endl; }
  
  dns_header* header = new dns_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(dns_header));

  if(DEBUG) { cout << "Done attaching DNS header, sending to UDP" << endl; }
  udp_send(message, DNS);
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

void ProcessPerMessage::udp_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received UDP message, attaching UDP header" << endl; }
  
  udp_header* header = new udp_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(udp_header));

  if(DEBUG) { cout << "Done attaching UDP header, sending to IP" << endl; }
  ip_send(message, UDP);
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

void ProcessPerMessage::ethernet_send(Message* message, int higher_level_protocol) {
  if(DEBUG) { cout << "Received ethernet message, attaching ethernet header" << endl; }
  
  ethernet_header* header = new ethernet_header;
  header->higher_level_protocol = higher_level_protocol;
  header->message_length = message->msgLen();

  message->msgAddHdr((char*) header, sizeof(ethernet_header));

  if(DEBUG) { cout << "Done attaching ethernet header" << endl; }
  cout << "message->msgLen(): " << message->msgLen() << endl;
}