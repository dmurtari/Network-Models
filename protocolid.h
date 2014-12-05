#ifndef _PROTOCOLID_
#define _PROTOCOLID_

#define ETHERNET 1
#define IP 2
#define TCP 3
#define UDP 4
#define FTP 5
#define TELNET 6
#define RDP 7
#define DNS 8
#define APPLICATION 9

/* Header structs for each of the headers */
struct ftp_header {
  int higher_level_protocol;
  char other_info[8];
  int message_length;           
};

struct telnet_header {
  int higher_level_protocol;
  char other_info[8];
  int message_length;           
};

struct rdp_header {
  int higher_level_protocol;
  char other_info[12];
  int message_length;           
};

struct dns_header {
  int higher_level_protocol;
  char other_info[8];
  int message_length;           
};

struct tcp_header {
  int higher_level_protocol;
  char other_info[4];
  int message_length;           
};

struct udp_header {
  int higher_level_protocol;
  char other_info[4];
  int message_length;           
};

struct ip_header {
  int higher_level_protocol;
  char other_info[12];
  int message_length;           
};

struct ethernet_header {
  int higher_level_protocol;
  char other_info[8];
  int message_length;           
};

#endif