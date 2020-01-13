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

#define NSFD_ICMP_NOT_SENT 0
#define NSFD_ICMP_SENT 1

bool IcmpClient::execute_request(std::string &domain)
{
  struct sockaddr_in addr_con{};

  char* ip_addr = resolve_dns((char*) domain.c_str(), &addr_con);
  if (ip_addr == nullptr)
  {
    printf("\nDNS lookup failed! Could not resolve hostname!\n");
//    return NSFD_ICMP_DNS_LOOKUP_FAILED;
    return false;
  }

  char* hostname = reverse_dns(ip_addr);
  if (hostname != nullptr)
  {
    printf("[DEBUG] received hostname %s for ip %s", hostname, ip_addr);
  }
  else
  {
//    return NSFD_ICMP_REQ_FAILED;
    return false;
  }

  printf("[DEBUG] Connecting to '%s' IP: %s\n", domain.c_str(), ip_addr);
  int raw_sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (raw_sock_fd < 0)
  {
    printf("[ERROR] Could not create raw socket for domain %s\n", domain.c_str());
//    return NSFD_ICMP_RAW_SOCK_CREATE_ERROR;
    return false;
  }

  // send pings continuously
  send_ping(raw_sock_fd, &addr_con, hostname, ip_addr, (char*) domain.c_str());

  free(hostname);
  free(ip_addr);

//  return NSFD_ICMP_REQ_SUCCESS;
  return true;
}

bool IcmpClient::send_ping(int raw_sock_fd, struct sockaddr_in* ping_addr, char* ping_dom, char* ping_ip, char* rev_host)
{
  int msg_count = 0;
  int msg_received_count = 0;

  struct icmp_packet_t packet{};
  struct sockaddr_in r_addr{};
  struct timespec time_start{};
  struct timespec time_end{};
  struct timespec tfs{};
  struct timespec tfe{};

  long double total_msec = 0;

  struct timeval tv_out{};
  tv_out.tv_sec = NSFD_ICMP_RECV_TIMEOUT;
  tv_out.tv_usec = 0;

  clock_gettime(CLOCK_MONOTONIC, &tfs);

  // SOL_IP - socket options level
  // ustawienie ttl-a dla pakietu IP
  int ttl_val = 64;
  if (setsockopt(raw_sock_fd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) < 0)
  {
    printf("[ERROR] Setting TTL failed\n");
    // TODO: zwrócić odpowiedni status
    return false;
  }

  // setting timeout of recv setting
  if (setsockopt(raw_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv_out, sizeof tv_out) < 0)
  {
    printf("[ERROR] Setting socket timeout failed\n");
    // TODO: zwrocic odpowieni status
    return false;
  }

  int retries = NSFD_ICMP_REQ_RETRIES;

  int sent = NSFD_ICMP_NOT_SENT;
  while (retries > 0 && sent == NSFD_ICMP_NOT_SENT)
  {
    retries--;

    // TODO: do czyszczenia pakietu wykorzystac memset
    bzero(&packet, sizeof(packet));

    packet.header.type = ICMP_ECHO;
    packet.header.un.echo.id = getpid();

    int i;
    for (i = 0; i < sizeof(packet.msg) - 1; i++)
    {
      packet.msg[i] = i + '0';
    }
    packet.msg[i] = 0;

    packet.header.un.echo.sequence = msg_count++;
    // TODO: wykorzystać istniejącą funkcję do checksumy z in_cksum.h
    packet.header.checksum = this->checksum(&packet, sizeof(packet));

    sleep(1);

    // send packet
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    if (sendto(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*) ping_addr, sizeof(*ping_addr)) == -1)
    {
      printf("[ERROR] Could not send ICMP packet.\n");
      sent = NSFD_ICMP_NOT_SENT;
    }
    else
    {
      sent = NSFD_ICMP_SENT;
    }

    int addr_len = sizeof(r_addr);
    if (recvfrom(raw_sock_fd, &packet, sizeof(packet), 0, (struct sockaddr*)&r_addr, (socklen_t*) &addr_len) <= 0 && msg_count > 1)
    {
      printf("[ERROR] Could not receive packet.\n");
    }
    else
    {
      clock_gettime(CLOCK_MONOTONIC, &time_end);

      double time = ((double) (time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
      long double rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + time;

      // if packet was not sent, don't receive
      if (sent)
      {
        if(!(packet.header.type == 69 && packet.header.code == 0))
        {
          printf("Error..Packet received with ICMP type %d code %d\n", packet.header.type, packet.header.code);
        }
        else
        {
          printf("\033[32;1m%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n\033[0m",
                 NSFD_ICMP_PKT_SIZE, ping_dom, rev_host, ping_ip, msg_count, ttl_val, rtt_msec);
          msg_received_count++;
        }
      }
    }
  }

  if (retries == 0 && sent == NSFD_ICMP_NOT_SENT)
    return false;

  clock_gettime(CLOCK_MONOTONIC, &tfe);
  double timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec))/1000000.0;

  total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0 + timeElapsed;

  printf("\033[32;1m%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\033[0m",
         msg_count, msg_received_count, ((msg_count - msg_received_count)/msg_count) * 100.0, total_msec);

  return true;
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



