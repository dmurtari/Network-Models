#include "processperprotocol.h"

#define DEBUG 1

using namespace std;

int main() {
  if(DEBUG) { cout << "Starting" << endl; }

  char message_contents[] = "wolololololololo";
  ProcessPerProtocol* ppp = new ProcessPerProtocol();
  Message* msg = new Message(message_contents, 100);

  if(DEBUG) { cout << "Creating message struct to send " << endl; }
  send_message pipe_send;
  pipe_send.protocol_id = APPLICATION;
  pipe_send.message = msg;

  if(DEBUG) { cout << "Sending message struct over FTP" << endl; }
  ppp->application_send_msg(pipe_send, FTP);
  // ppp->application_send_msg(pipe_send, TELNET);
  // ppp->application_send_msg(pipe_send, RDP);
  // ppp->application_send_msg(pipe_send, DNS);
  while(1){}
}