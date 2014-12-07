#include "processpermessage.h"

#define DEBUG 0

using namespace std;

void* ftp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:FTP:FTP message sent over PPM", i);

    Message* ftp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(ftp_message, FTP); 
    usleep(100);
  }

  return (void*) ppm;
}

void* telnet_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:Telnet:Telnet message sent over PPM", i);

    Message* telnet_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(telnet_message, TELNET); 
    usleep(100);
  }

  return (void*) ppm;
}

void* rdp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:RDP:RDP message sent over PPM", i);

    Message* rdp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(rdp_message, RDP); 
    usleep(100);
  }

  return (void*) ppm;
}

void* dns_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[100];
    sprintf(message_contents, "%d:DNS:DNS message sent over PPM", i);

    Message* dns_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(dns_message, DNS); 
    usleep(100);
  }
  
  return (void*) ppm;
}

int main() {
  pthread_t message_applications[8];
  struct timeval start, end;
  long mtime, seconds, useconds;

  gettimeofday(&start, NULL);
  ProcessPerMessage* ppm1 = new ProcessPerMessage("12123", "12124");
  ProcessPerMessage* ppm2 = new ProcessPerMessage("12124", "12123");

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  pthread_create(&message_applications[0], NULL, ftp_app, (void*) ppm1);
  pthread_create(&message_applications[1], NULL, ftp_app, (void*) ppm2);
  pthread_create(&message_applications[2], NULL, telnet_app, (void*) ppm1);
  pthread_create(&message_applications[3], NULL, telnet_app, (void*) ppm2);
  pthread_create(&message_applications[4], NULL, rdp_app, (void*) ppm1);
  pthread_create(&message_applications[5], NULL, rdp_app, (void*) ppm2);
  pthread_create(&message_applications[6], NULL, dns_app, (void*) ppm1);
  pthread_create(&message_applications[7], NULL, dns_app, (void*) ppm2);
  
  pthread_join(message_applications[0], NULL);
  pthread_join(message_applications[1], NULL);
  pthread_join(message_applications[2], NULL);
  pthread_join(message_applications[3], NULL);
  pthread_join(message_applications[4], NULL);
  pthread_join(message_applications[5], NULL);
  pthread_join(message_applications[6], NULL);
  pthread_join(message_applications[7], NULL);

  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;

  mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  printf("Elapsed time for Process per Message: %ld milleseconds\n", mtime);

  return 1;
}