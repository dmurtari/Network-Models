#include "processpermessage.h"

#define DEBUG 0

using namespace std;

void ftp_app(void* arg) {
  ProcessPerMessage* ppm = (ProcessPerMessage*) arg;

  for(int i = 0; i < 1; i++) {
    char message_contents[] = "This is an FTP message";
    Message* ftp_message = new Message(message_contents, 100);
    
    ppm->application_send_msg(ftp_message, FTP); 
    usleep(100);
  }
}

int main() {
  ThreadPool* message_applications = new ThreadPool(4);
  ProcessPerMessage* ppm = new ProcessPerMessage();

  sleep(1);

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  message_applications->dispatch_thread(ftp_app, (void*) ppm);
  // message_applications->dispatch_thread(telnet_app, (void*) ppp);
  // message_applications->dispatch_thread(rdp_app, (void*) ppp);
  // message_applications->dispatch_thread(dns_app, (void*) ppp);
  while(1);
}