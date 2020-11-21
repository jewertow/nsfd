#ifndef NSFD_DNS_DNSRESOLVER_H_
#define NSFD_DNS_DNSRESOLVER_H_

#include <string>

#define AVAILABLE_PORT 0

std::string resolve_dns(char* host_addr, struct sockaddr_in* addr_con);

std::string reverse_dns(char* ip_addr);

#endif // NSFD_DNS_DNSRESOLVER_H_
