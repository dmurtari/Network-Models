#include "processperprotocol.h"

#define DEBUG 0

using namespace std;

void* ftp_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:FTP:FTP message sent over PPP", i);

    Message* ftp_message = new Message(message_contents, 100);
    send_message ftp_container;

    ftp_container.protocol_id = APPLICATION;
    ftp_container.message = ftp_message;

    if(DEBUG) { cout << "Sending FTP message " << i << endl; }
    ppp->application_send_msg(ftp_container, FTP); 
    usleep(100);
  }

  return (void*) ppp;
}

void* telnet_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:Telnet:Telnet message sent over PPP", i);

    Message* telnet_message = new Message(message_contents, 100);
    send_message telnet_container;

    telnet_container.protocol_id = APPLICATION;
    telnet_container.message = telnet_message;

    if(DEBUG) { cout << "Sending telnet message " << i << endl; }
    ppp->application_send_msg(telnet_container, TELNET); 
    usleep(100);
  }

  return (void*) ppp;
}

void* rdp_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:RDP:RDP message sent over PPP", i);

    Message* rdp_message = new Message(message_contents, 100);
    send_message rdp_container;

    rdp_container.protocol_id = APPLICATION;
    rdp_container.message = rdp_message;

    if(DEBUG) { cout << "Sending rdp message " << i << endl; }
    ppp->application_send_msg(rdp_container, RDP); 
    usleep(100);
  }

  return (void*) ppp;
}

void* dns_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:DNS:DNS message sent over PPP", i);

    Message* dns_message = new Message(message_contents, 100);
    send_message dns_container;

    dns_container.protocol_id = APPLICATION;
    dns_container.message = dns_message;

    if(DEBUG) { cout << "Sending DNS message " << i << endl; }
    ppp->application_send_msg(dns_container, DNS); 
    usleep(100);
  }

  return (void*) ppp;
}

int main() {
  pthread_t protocol_applications[8];
  struct timeval start, end;
  long mtime, seconds, useconds;

  gettimeofday(&start, NULL);
  ProcessPerProtocol* ppp1 = new ProcessPerProtocol("12123", "12124");
  ProcessPerProtocol* ppp2 = new ProcessPerProtocol("12124", "12123");

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  pthread_create(&protocol_applications[0], NULL, ftp_app, (void*) ppp1);
  pthread_create(&protocol_applications[1], NULL, ftp_app, (void*) ppp2);
  pthread_create(&protocol_applications[2], NULL, telnet_app, (void*) ppp1);
  pthread_create(&protocol_applications[3], NULL, telnet_app, (void*) ppp2);
  pthread_create(&protocol_applications[4], NULL, rdp_app, (void*) ppp1);
  pthread_create(&protocol_applications[5], NULL, rdp_app, (void*) ppp2);
  pthread_create(&protocol_applications[6], NULL, dns_app, (void*) ppp1);
  pthread_create(&protocol_applications[7], NULL, dns_app, (void*) ppp2);
  
  pthread_join(protocol_applications[0], NULL);
  pthread_join(protocol_applications[1], NULL);
  pthread_join(protocol_applications[2], NULL);
  pthread_join(protocol_applications[3], NULL);
  pthread_join(protocol_applications[4], NULL);
  pthread_join(protocol_applications[5], NULL);
  pthread_join(protocol_applications[6], NULL);
  pthread_join(protocol_applications[7], NULL);
  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;

  mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  printf("Elapsed time for Process per Protocol: %ld milleseconds\n", mtime);

  return 1;
}