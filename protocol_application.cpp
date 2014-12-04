#include "processperprotocol.h"

#define DEBUG 1

using namespace std;

void ftp_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an FTP message";
    Message* ftp_message = new Message(message_contents, 100);
    send_message ftp_container;

    ftp_container.protocol_id = APPLICATION;
    ftp_container.message = ftp_message;

    if(DEBUG) { cout << "Sending FTP message " << i << endl; }
    ppp->application_send_msg(ftp_container, FTP); 
    usleep(100);
  }
}

void telnet_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is a telnet message";
    Message* telnet_message = new Message(message_contents, 100);
    send_message telnet_container;

    telnet_container.protocol_id = APPLICATION;
    telnet_container.message = telnet_message;

    if(DEBUG) { cout << "Sending telnet message " << i << endl; }
    ppp->application_send_msg(telnet_container, TELNET); 
    usleep(100);
  }
}

void rdp_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is a RDP message";
    Message* rdp_message = new Message(message_contents, 100);
    send_message rdp_container;

    rdp_container.protocol_id = APPLICATION;
    rdp_container.message = rdp_message;

    if(DEBUG) { cout << "Sending RDP message " << i << endl; }
    ppp->application_send_msg(rdp_container, RDP); 
    usleep(100);
  }
}

void dns_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an DNS message";
    Message* dns_message = new Message(message_contents, 100);
    send_message dns_container;

    dns_container.protocol_id = APPLICATION;
    dns_container.message = dns_message;

    if(DEBUG) { cout << "Sending DNS message " << i << endl; }
    ppp->application_send_msg(dns_container, DNS); 
    usleep(100);
  }
}

int main() {
  ThreadPool* protocol_applications = new ThreadPool(4);
  ProcessPerProtocol* ppp = new ProcessPerProtocol();

  sleep(1);

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  protocol_applications->dispatch_thread(ftp_app, (void*) ppp);
  protocol_applications->dispatch_thread(telnet_app, (void*) ppp);
  protocol_applications->dispatch_thread(rdp_app, (void*) ppp);
  protocol_applications->dispatch_thread(dns_app, (void*) ppp);
  
}