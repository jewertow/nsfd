#include "dns.h"
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string>

std::string resolve_dns(char* host_addr, struct sockaddr_in* addr_con)
{
  fprintf(stdout, "Resolving %s address\n", host_addr);

  struct hostent* host_entity;
  if ((host_entity = gethostbyname(host_addr)) == nullptr) {
    fprintf(stdout, "Failed to resolve dns address %s\n", host_addr);
    return std::string();
  }

  char* resolved_ip = inet_ntoa(*(struct in_addr*) host_entity->h_addr);
  fprintf(stdout, "Resolved ip = %s\n", resolved_ip);

  (*addr_con).sin_family = host_entity->h_addrtype;
  (*addr_con).sin_port = htons(AVAILABLE_PORT);
  (*addr_con).sin_addr.s_addr  = *(long*) host_entity->h_addr;

  return std::string(resolved_ip);
}

std::string reverse_dns(char* ip_addr)
{
  struct sockaddr_in temp_addr{};
  char hostname_buffer[NI_MAXHOST];

  temp_addr.sin_family = AF_INET;
  temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
  socklen_t len = sizeof(struct sockaddr_in);

  // NI_NAMEREQD - returns error in case of unknown hostname
  int err = getnameinfo((struct sockaddr*) &temp_addr, len, hostname_buffer, sizeof(hostname_buffer), nullptr, 0, NI_NAMEREQD);
  if (err) {
    fprintf(stdout, "Could not resolve reverse lookup of hostname\n");
    return std::string("unknown");
  }

  return std::string(hostname_buffer);
}
