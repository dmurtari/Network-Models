#include "processperprotocol.h"

using namespace std;

ProcessPerProtocol::ProcessPerProtocol() {
  
  /* Create pipes */
  pipe(ftp_send_pipe.pipes); pipe(ftp_receive_pipe.pipes);
  pipe(telnet_send_pipe.pipes); pipe(telnet_receive_pipe.pipes);
  pipe(rdp_send_pipe.pipes); pipe(rdp_receive_pipe.pipes);
  pipe(dns_send_pipe.pipes); pipe(dns_receive_pipe.pipes);
  pipe(tcp_send_pipe.pipes); pipe(tcp_receive_pipe.pipes);
  pipe(ip_send_pipe.pipes);  pipe(ip_receive_pipe.pipes);
  pipe(ethernet_send_pipe.pipes); pipe(ethernet_receive_pipe.pipes);

}

int main() {
  ProcessPerProtocol* ppp = new ProcessPerProtocol();
  return 0;
}