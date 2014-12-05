#include "processpermessage.h"

#define DEBUG 0

using namespace std;

void ftp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an FTP message";
    Message* ftp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(ftp_message, FTP); 
    usleep(100);
  }
}

void telnet_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an telnet message";
    Message* telnet_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(telnet_message, TELNET); 
    usleep(100);
  }
}

void rdp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an RDP message";
    Message* rdp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(rdp_message, RDP); 
    usleep(100);
  }
}

void dns_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 100; i++) {
    char message_contents[] = "This is an DNS message";
    Message* dns_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(dns_message, DNS); 
    usleep(100);
  }
}

int main() {
  ThreadPool* message_applications = new ThreadPool(4);
  ProcessPerMessage* ppm1 = new ProcessPerMessage("12123", "12124");
  ProcessPerMessage* ppm2 = new ProcessPerMessage("12124", "12123");

  sleep(1);

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  message_applications->dispatch_thread(ftp_app, (void*) ppm1);
  message_applications->dispatch_thread(telnet_app, (void*) ppm1);
  message_applications->dispatch_thread(rdp_app, (void*) ppm1);
  message_applications->dispatch_thread(dns_app, (void*) ppm1);
  while(1);
}