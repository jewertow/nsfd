#include "icmp_client.h"
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

  std::string ip_addr = resolve_dns((char*) domain.c_str(), &addr_con);
  if (ip_addr.empty()) {
    fprintf(stdout, "DNS lookup failed - could not resolve hostname %s", domain.c_str());
    return this->failed_request();
  }

  std::string hostname = reverse_dns((char*) ip_addr.c_str());
  fprintf(stdout, "Received hostname %s for ip %s\n", hostname.c_str(), ip_addr.c_str());

  fprintf(stdout, "Connecting to '%s' IP: %s\n", domain.c_str(), ip_addr.c_str());
  int raw_sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (raw_sock_fd < 0) {
    fprintf(stdout, "Failed to create raw socket for domain %s\n", domain.c_str());
    return this->failed_request();
  }

  return send_icmp(raw_sock_fd, &addr_con, (char*) hostname.c_str(), (char*) ip_addr.c_str(), (char*) domain.c_str());
}

IcmpResult* IcmpClient::send_icmp(int raw_sock_fd, struct sockaddr_in* dest_addr, char* hostname, char* dest_ip, char* domain_name)
{
  struct timespec time_start{};
  struct timespec time_end{};

  // SOL_IP - socket options level
  int ttl = 64;
  if (setsockopt(raw_sock_fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
    fprintf(stdout, "Failed to set TTL for ICMP request [domain=%s]\n", domain_name);
    return this->failed_request();
  }

  struct timeval timeout{};
  timeout.tv_sec = NSFD_ICMP_RECV_TIMEOUT;
  timeout.tv_usec = 0;
  if (setsockopt(raw_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, sizeof timeout) < 0) {
    fprintf(stdout, "Failed to set socket timeout for ICMP request [domain=%s]\n", domain_name);
    return this->failed_request();
  }

  // TODO: use memset
  struct IcmpPacket packet{};
  bzero(&packet, sizeof(packet));
  packet.header.type = ICMP_ECHO;
  packet.header.un.echo.id = getpid();
  packet.header.un.echo.sequence = 1;
  fill_packet_with_random_data(packet.msg, sizeof(packet.msg));

  packet.header.checksum = this->checksum(&packet, sizeof(packet));

  // send packet
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  if (sendto(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) dest_addr, sizeof(*dest_addr)) == -1) {
    fprintf(stdout, "Failed to send ICMP packet to %s\n", domain_name);
    return failed_request();
  }

  struct sockaddr_in src_addr{};
  int addr_len = sizeof(src_addr);
  if (recvfrom(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) &src_addr, (socklen_t*) &addr_len) <= 0) {
    fprintf(stdout, "Failed to receive ICMP packet from %s\n", domain_name);
    return this->failed_request();
  } else {
    clock_gettime(CLOCK_MONOTONIC, &time_end);

    double time_ns = ((double) (time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
    long double rtt = (long double) (time_end.tv_sec - time_start.tv_sec) * 1000.0 + time_ns;

    fprintf(stdout, "\033[32;1m%d bytes from hostname: %s, domain name: (%s) ip: (%s) ttl=%d rtt = %Lf ms.\n\033[0m",
           NSFD_ICMP_PKT_SIZE, hostname, domain_name, dest_ip, ttl, rtt);

    return new IcmpResult{
      .success = true,
      .rtt_ms = rtt
    };
  }
}

// TODO: look for a function for checksum in in_cksum.h
unsigned short IcmpClient::checksum(void* bytes, int len)
{
  auto* buf = (unsigned short*) bytes;
  unsigned int sum = 0;

  for (sum = 0; len > 1; len -= 2 ) {
    sum += *buf++;
  }

  if (len == 1) {
    sum += *(unsigned char *) buf;
  }

  sum = (sum >> 16u) + (sum & 0xFFFFu);
  sum += (sum >> 16u);

  return ~sum;
}

IcmpResult* IcmpClient::failed_request()
{
  return new IcmpResult{
    .success = false,
    .rtt_ms = -1
  };
}

void IcmpClient::fill_packet_with_random_data(char* packet_msg, int size)
{
  for (int i = 0; i < size; i++) {
    packet_msg[i] = i + '0';
  }
}
