#include "processperprotocol.h"

#define DEBUG 1

using namespace std;

ProcessPerProtocol::ProcessPerProtocol() {

  if(DEBUG) { cout << "Creating ThreadPool" << endl; }
  protocol_threads = new ThreadPool(16);
  

  /* Create pipes */
  if(DEBUG) { cout << "Creating Pipes" << endl; }
  pipe(ftp_send_pipe.pipes); pipe(ftp_receive_pipe.pipes);
  pipe(telnet_send_pipe.pipes); pipe(telnet_receive_pipe.pipes);
  pipe(rdp_send_pipe.pipes); pipe(rdp_receive_pipe.pipes);
  pipe(dns_send_pipe.pipes); pipe(dns_receive_pipe.pipes);
  pipe(tcp_send_pipe.pipes); pipe(tcp_receive_pipe.pipes);
  pipe(ip_send_pipe.pipes);  pipe(ip_receive_pipe.pipes);
  pipe(ethernet_send_pipe.pipes); pipe(ethernet_receive_pipe.pipes);

  /* Initialize mutexes to control access to pipes */
  if(DEBUG) { cout << "Initializing Mutexes" << endl; }
  pthread_mutex_init(&ftp_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(ftp_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(telnet_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(telnet_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(rdp_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(rdp_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(dns_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(dns_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(tcp_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(tcp_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(udp_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(udp_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(ip_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(ip_receive_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(ethernet_send_pipe.pipe_mutex, NULL);
  // pthread_mutex_init(ethernet_receive_pipe.pipe_mutex, NULL);

  if(DEBUG) { cout << "Dispatching Threads" << endl; }
  protocol_threads->dispatch_thread(ftp_send, (void*) this);
}

void ProcessPerProtocol::application_send_msg(send_message message) {
  switch(message.protocol_id){
    case FTP:
      if(DEBUG) { cout << "Locking and writing to FTP Send" << endl; }
      pthread_mutex_lock(&ftp_send_pipe.pipe_mutex);
      write(ftp_send_pipe.pipes[1], message.message, sizeof(send_message));
      pthread_mutex_unlock(&ftp_send_pipe.pipe_mutex);
      if(DEBUG) { cout << "Done writing to FTP Send" << endl; }
      break;
  }
}

void ProcessPerProtocol::ftp_send(void* arg) {
  ProcessPerProtocol* ppp = (ProcessPerProtocol*) arg;

  while(1) {
    send_message* read_from_pipe = new send_message;

    if(DEBUG) { cout << "Locking and reading from FTP Send" << endl; }
    pthread_mutex_lock(&ppp->ftp_send_pipe.pipe_mutex);
    read(ppp->ftp_send_pipe.pipes[0], read_from_pipe, sizeof(send_message));
    pthread_mutex_unlock(&ppp->ftp_send_pipe.pipe_mutex);
    if(DEBUG) { cout << "Done reading from FTP Send" << endl; }

  }
}

