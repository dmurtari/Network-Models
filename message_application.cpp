#include "processpermessage.h"

#define DEBUG 0

using namespace std;

int main() {
  ThreadPool* message_applications = new ThreadPool(4);
  ProcessPerMessage* ppm = new ProcessPerMessage();

  sleep(1);

  if(DEBUG) { cout << "Dispatching application threads" << endl; }
  // message_applications->dispatch_thread(ftp_app, (void*) ppp);
  // message_applications->dispatch_thread(telnet_app, (void*) ppp);
  // message_applications->dispatch_thread(rdp_app, (void*) ppp);
  // message_applications->dispatch_thread(dns_app, (void*) ppp);
  
}