#include "protocolid.h"
#include "message.h"

/* Container to hold pointer to message and the protocol sending the 
   message */
struct send_message {
  int protocol_id;
  Message *message;
};