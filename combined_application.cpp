#include "processpermessage.h"
#include "processperprotocol.h"

#define DEBUG 0

using namespace std;

void* ppm_ftp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an FTP message";
    Message* ftp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(ftp_message, FTP); 
    usleep(100);
  }

  return (void*) ppm;
}

void* ppm_telnet_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an telnet message";
    Message* telnet_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(telnet_message, TELNET); 
    usleep(100);
  }

  return (void*) ppm;
}

void* ppm_rdp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an RDP message";
    Message* rdp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(rdp_message, RDP); 
    usleep(100);
  }

  return (void*) ppm;
}

void* ppm_dns_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an DNS message";
    Message* dns_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(dns_message, DNS); 
    usleep(100);
  }

  return (void*) ppm;
}

void* ppp_ftp_app(void* arg) {
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

  return (void*) ppp;
}

void* ppp_telnet_app(void* arg) {
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

  return (void*) ppp;
}

void* ppp_rdp_app(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an RDP message";
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

void* ppp_dns_app(void* arg) {
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

  return (void*) ppp;
}

int main() {
  pthread_t combined_applications[8];
  struct timeval start, end;
  long mtime, seconds, useconds;

  gettimeofday(&start, NULL);
  ProcessPerMessage* ppm3 = new ProcessPerMessage("12127", "12128");
  ProcessPerProtocol* ppp3 = new ProcessPerProtocol("12128", "12127");

  if(DEBUG) { cout << "Dispatching combined application threads" << endl; }
  pthread_create(&combined_applications[0], NULL, ppm_ftp_app, (void*) ppm3);
  pthread_create(&combined_applications[1], NULL, ppp_ftp_app, (void*) ppp3);
  pthread_create(&combined_applications[2], NULL, ppm_telnet_app, (void*) ppm3);
  pthread_create(&combined_applications[3], NULL, ppp_telnet_app, (void*) ppp3);
  pthread_create(&combined_applications[4], NULL, ppm_rdp_app, (void*) ppm3);
  pthread_create(&combined_applications[5], NULL, ppp_rdp_app, (void*) ppp3);
  pthread_create(&combined_applications[6], NULL, ppm_dns_app, (void*) ppm3);
  pthread_create(&combined_applications[7], NULL, ppp_dns_app, (void*) ppp3);
  
  pthread_join(combined_applications[0], NULL);
  pthread_join(combined_applications[1], NULL);
  pthread_join(combined_applications[2], NULL);
  pthread_join(combined_applications[3], NULL);
  pthread_join(combined_applications[4], NULL);
  pthread_join(combined_applications[5], NULL);
  pthread_join(combined_applications[6], NULL);
  pthread_join(combined_applications[7], NULL);
  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;

  mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  printf("Elapsed time for combined: %ld milleseconds\n", mtime);


  return 1;
}