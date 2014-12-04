#include "processpermessage.h"

#define DEBUG 1

ProcessPerMessage::ProcessPerMessage() {
  if(DEBUG) { cout << "Contructing ProcessPerMessage Object" << endl; }
  message_threads = new ThreadPool(10);
}

ProcessPerMessage::~ProcessPerMessage() {
  
}