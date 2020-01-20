#include "IcmpClient.h"
#include "dns.h"
#include "../util/CmdColor.h"

#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>

#define NSFD_ICMP_NOT_SENT 0
#define NSFD_ICMP_SENT 1

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
  int msg_count = 0;
  int msg_received_count = 0;

  struct icmp_packet_t packet{};
  struct timespec time_start{};
  struct timespec time_end{};
  struct timespec total_time_start{};
  struct timespec total_time_end{};

  long double total_msec = 0;

  struct timeval timeout{};
  timeout.tv_sec = NSFD_ICMP_RECV_TIMEOUT;
  timeout.tv_usec = 0;

  clock_gettime(CLOCK_MONOTONIC, &total_time_start);

  // SOL_IP - socket options level
  int ttl = 64;
  if (setsockopt(raw_sock_fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
  {
    fprintf(stdout, "[ERROR] Setting TTL failed\n");
    return this->failed_request();
  }

  // setting timeout of recv setting
  if (setsockopt(raw_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, sizeof timeout) < 0)
  {
    fprintf(stdout, "[ERROR] Setting socket timeout failed\n");
    return this->failed_request();
  }

  IcmpResult* result = nullptr;

  int retries = NSFD_ICMP_REQ_RETRIES;
  int sent = NSFD_ICMP_NOT_SENT;
  while (retries > 0 && sent == NSFD_ICMP_NOT_SENT)
  {
    retries--;

    // TODO: do czyszczenia pakietu wykorzystac memset
    bzero(&packet, sizeof(packet));

    packet.header.type = ICMP_ECHO;
    packet.header.un.echo.id = getpid();

    // uzupelnienie tablicy msg losowymi danymi
    for (int i = 0; i < sizeof(packet.msg); i++)
    {
      packet.msg[i] = i + '0';
    }

    packet.header.un.echo.sequence = msg_count++;
    // TODO: poszukać gotową funkcję do sumy kontrolnej np w in_cksum.h
    packet.header.checksum = this->checksum(&packet, sizeof(packet));

    // send packet
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    if (sendto(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) dest_addr, sizeof(*dest_addr)) == -1)
    {
      printf("[ERROR] Could not send ICMP packet.\n");
      sent = NSFD_ICMP_NOT_SENT;
      sleep(NSFD_ICMP_BACKOFF_TIME_S);
    }
    else
    {
      sent = NSFD_ICMP_SENT;
    }

    struct sockaddr_in src_addr{};
    int addr_len = sizeof(src_addr);
    ssize_t recv_res = recvfrom(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) &src_addr, (socklen_t*) &addr_len);
    if (recv_res <= 0 && msg_count > 1)
    {
      printf("[ERROR] Could not receive packet.\n");
    }
    else
    {
      clock_gettime(CLOCK_MONOTONIC, &time_end);

      double time = ((double) (time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
      long double rtt_ms = (long double) (time_end.tv_sec - time_start.tv_sec) * 1000.0 + time;

      result = new IcmpResult{};
      result->success = true;
      result->rtt_ms = rtt_ms;

      // if packet was not sent, don't receive
      if (sent && recv_res)
      {
        printf("\033[32;1m%d bytes from hostname: %s, domain name: (%s) ip: (%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n\033[0m",
               NSFD_ICMP_PKT_SIZE, hostname, rev_host, ping_ip, msg_count, ttl, rtt_ms);
        msg_received_count++;
      }
      else
      {
        printf("%s[WARN] ICMP sent, not received%s\n", RED, RESET);
      }
    }
  }

  if (retries == 0 && sent == NSFD_ICMP_NOT_SENT)
    return this->failed_request();

  clock_gettime(CLOCK_MONOTONIC, &total_time_end);
  double time_elapsed = ((double)(total_time_end.tv_nsec - total_time_start.tv_nsec)) / 1000000.0;

  total_msec = (total_time_end.tv_sec - total_time_start.tv_sec) * 1000.0 + time_elapsed;

  printf("\033[32;1m%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\033[0m",
         msg_count, msg_received_count, ((msg_count - msg_received_count)/msg_count) * 100.0, total_msec);

  return result;
}

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
