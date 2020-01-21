#include "IcmpClient.h"
#include "dns.h"

#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>

IcmpResult* IcmpClient::execute_request(const std::string& domain)
{
  struct sockaddr_in addr_con{};

  char* ip_addr = resolve_dns((char*) domain.c_str(), &addr_con);
  if (ip_addr == nullptr)
  {
    printf("\nDNS lookup failed! Could not resolve hostname!\n");
    return this->failed_request();
  }

  char* hostname = reverse_dns(ip_addr);
  if (hostname != nullptr)
  {
    printf("[DEBUG] received hostname %s for ip %s", hostname, ip_addr);
  }
  else
  {
    return this->failed_request();
  }

  printf("[DEBUG] Connecting to '%s' IP: %s\n", domain.c_str(), ip_addr);
  int raw_sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (raw_sock_fd < 0)
  {
    printf("[ERROR] Could not create raw socket for domain %s\n", domain.c_str());
    return this->failed_request();
  }

  auto* icmp_result = send_icmp(raw_sock_fd, &addr_con, hostname, ip_addr, (char*) domain.c_str());

  free(hostname);
  free(ip_addr);
  
  return icmp_result;
}

IcmpResult* IcmpClient::send_icmp(int raw_sock_fd, struct sockaddr_in* dest_addr, char* hostname, char* ping_ip, char* rev_host)
{
  struct timespec time_start{};
  struct timespec time_end{};

  // SOL_IP - socket options level
  int ttl = 64;
  if (setsockopt(raw_sock_fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
  {
    fprintf(stdout, "[ERROR] Setting TTL failed\n");
    return this->failed_request();
  }

  struct timeval timeout{};
  timeout.tv_sec = NSFD_ICMP_RECV_TIMEOUT;
  timeout.tv_usec = 0;
  // setting timeout of recv setting
  if (setsockopt(raw_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, sizeof timeout) < 0)
  {
    fprintf(stdout, "[ERROR] Setting socket timeout failed\n");
    return this->failed_request();
  }

  // TODO: do czyszczenia pakietu wykorzystac memset
  struct IcmpPacket packet{};
  bzero(&packet, sizeof(packet));
  packet.header.type = ICMP_ECHO;
  packet.header.un.echo.id = getpid();
  packet.header.un.echo.sequence = 1;
  fill_packet_with_random_data(packet.msg, sizeof(packet.msg));

  packet.header.checksum = this->checksum(&packet, sizeof(packet));

  // send packet
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  if (sendto(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) dest_addr, sizeof(*dest_addr)) == -1)
  {
    printf("[ERROR] Could not send ICMP packet.\n");
    return failed_request();
  }

  struct sockaddr_in src_addr{};
  int addr_len = sizeof(src_addr);
  ssize_t recv_res = recvfrom(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) &src_addr, (socklen_t*) &addr_len);
  if (recv_res <= 0)
  {
    printf("[ERROR] Could not receive ICMP packet.\n");
    return this->failed_request();
  }
  else
  {
    clock_gettime(CLOCK_MONOTONIC, &time_end);

    double time_ns = ((double) (time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
    long double rtt = (long double) (time_end.tv_sec - time_start.tv_sec) * 1000.0 + time_ns;

    printf("\033[32;1m%d bytes from hostname: %s, domain name: (%s) ip: (%s) ttl=%d rtt = %Lf ms.\n\033[0m",
           NSFD_ICMP_PKT_SIZE, hostname, rev_host, ping_ip, ttl, rtt);

    return new IcmpResult{
      .success = true,
      .rtt_ms = rtt
    };
  }
}

// TODO: poszukać gotową funkcję do sumy kontrolnej np w in_cksum.h
unsigned short IcmpClient::checksum(void* bytes, int len)
{
  auto* buf = (unsigned short*) bytes;
  unsigned int sum = 0;

  for (sum = 0; len > 1; len -= 2 )
    sum += *buf++;

  if (len == 1)
    sum += *(unsigned char*) buf;

  sum = (sum >> 16u) + (sum & 0xFFFFu);
  sum += (sum >> 16u);

  return ~sum;
}

IcmpResult* IcmpClient::failed_request()
{
  auto* result = new IcmpResult{};
  result->success = false;
  return result;
}

void IcmpClient::fill_packet_with_random_data(char* packet_msg, int size)
{
  for (int i = 0; i < size; i++)
  {
    packet_msg[i] = i + '0';
  }
}
