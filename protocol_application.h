#ifndef _PROTOCOLAPPLICATION_
#define _PROTOCOLAPPLICATION_


#define SERVICE_PORT 12153

/* Container to hold pointer to message and the protocol sending the 
   message */
struct send_message {
  int protocol_id;
  Message *message;
};

#endif