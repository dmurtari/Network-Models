#include "processperprotocol.h"

using namespace std;

ProcessPerProtocol::ProcessPerProtocol() {
  
  protocol_threads = new ThreadPool(16);
  
  /* Create pipes */
  pipe(ftp_send_pipe.pipes); pipe(ftp_receive_pipe.pipes);
  pipe(telnet_send_pipe.pipes); pipe(telnet_receive_pipe.pipes);
  pipe(rdp_send_pipe.pipes); pipe(rdp_receive_pipe.pipes);
  pipe(dns_send_pipe.pipes); pipe(dns_receive_pipe.pipes);
  pipe(tcp_send_pipe.pipes); pipe(tcp_receive_pipe.pipes);
  pipe(ip_send_pipe.pipes);  pipe(ip_receive_pipe.pipes);
  pipe(ethernet_send_pipe.pipes); pipe(ethernet_receive_pipe.pipes);

  /* Initialize mutexes to control access to pipes */
  pthread_mutex_init(&ftp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ftp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&telnet_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&telnet_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&rdp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&rdp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&dns_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&dns_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&tcp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&tcp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&udp_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&udp_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ip_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ip_receive_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ethernet_send_pipe.pipe_mutex, NULL);
  pthread_mutex_init(&ethernet_receive_pipe.pipe_mutex, NULL);

}

int main() {
  ProcessPerProtocol* ppp = new ProcessPerProtocol();
  return 0;
}