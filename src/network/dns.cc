#include "dns.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

char* resolve_dns(char* host_addr, struct sockaddr_in* addr_con)
{
  printf("[DEBUG] Resolving %s address\n", host_addr);
  char* ip = (char*) malloc(NI_MAXHOST * sizeof(char));

  struct hostent* host_entity;
  if ((host_entity = gethostbyname(host_addr)) == nullptr)
  {
    printf("[ERROR] Could not resolve dns address %s\n", host_addr);
    return nullptr;
  }

  strcpy(ip, inet_ntoa(*(struct in_addr*) host_entity->h_addr));

  // TODO: Wyniesc do icmp
  (*addr_con).sin_family = host_entity->h_addrtype;
  (*addr_con).sin_port = htons(AVAILABLE_PORT);
  (*addr_con).sin_addr.s_addr  = *(long*) host_entity->h_addr;

  return ip;
}

char* reverse_dns(char* ip_addr)
{
  struct sockaddr_in temp_addr{};
  char buf[NI_MAXHOST];

  temp_addr.sin_family = AF_INET;
  temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
  socklen_t len = sizeof(struct sockaddr_in);

  // NI_NAMEREQD - zwróci błąd jeśli nie rozpozna nazwy hosta
  if (getnameinfo((struct sockaddr*) &temp_addr, len, buf, sizeof(buf), nullptr, 0, NI_NAMEREQD))
  {
    printf("Could not resolve reverse lookup of hostname\n");
    return nullptr;
  }

  char* ret_buf = (char*) malloc((strlen(buf) +1) * sizeof(char));
  strcpy(ret_buf, buf);

  return ret_buf;
}

