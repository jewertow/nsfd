#ifndef NSFD_ICMP_ICMPCLIENT_H_
#define NSFD_ICMP_ICMPCLIENT_H_

#include <string>
#include <stdlib.h>
#include <netinet/ip_icmp.h>

#define NSFD_ICMP_REQ_SUCCESS 0
#define NSFD_ICMP_REQ_FAILED -1
#define NSFD_ICMP_DNS_LOOKUP_FAILED -2
#define NSFD_ICMP_RAW_SOCK_CREATE_ERROR -3

#define NSFD_ICMP_PKT_SIZE 64
#define NSFD_ICMP_RECV_TIMEOUT 1

#define NSFD_ICMP_REQ_RETRIES 10
#define NSFD_ICMP_BACKOFF_TIME_S 1

// ping packet structure
struct icmp_packet_t
{
  struct icmphdr header;
  char msg[NSFD_ICMP_PKT_SIZE - sizeof(struct icmphdr)];
};

struct IcmpResult
{
  bool success;
  long double rtt_ms;
};

class IcmpClient
{
private:
  unsigned short checksum(void* bytes, int length);
  IcmpResult* send_icmp(int raw_sock_fd, struct sockaddr_in* dest_addr, char* hostname, char* ping_ip, char* rev_host);
  IcmpResult* failed_request();
public:
  /**
   * Zwraca true jeśli udaje się nawiązać połączenie z hostem i false w przeciwnym wypadku.
   * Metoda loguje w konsoli informacje o połączeniu.
   */
  IcmpResult* execute_request(const std::string& domain);
};

#endif // NSFD_ICMP_ICMPCLIENT_H_
