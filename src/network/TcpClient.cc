#include "TcpClient.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

bool TcpClient::execute_request(std::string &addr, int port)
{
  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
  {
    fprintf(stderr, "[ERROR] Cannot create a client socket.\n");
    // TODO: Wyniesc status do jakiejś definicji
    return -1;
  }

  struct hostent* host_entity;
  if ((host_entity = gethostbyname(addr.c_str())) == nullptr)
  {
    fprintf(stderr, "[ERROR] Could not resolve dns address %s\n", addr.c_str());
    return false;
  }

//  struct hostent* ip = inet_ntoa(*(struct in_addr*) host_entity->h_addr);

//  char ip[NI_MAXHOST];
//  strcpy(ip, inet_ntoa(*(struct in_addr*) host_entity->h_addr));

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  memcpy(&server_addr.sin_addr.s_addr, host_entity->h_addr, host_entity->h_length);

  int conn_result = connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr));
  if (conn_result < 0)
  {
    fprintf(stderr, "Cannot connect to the server (%s:%i).\n", addr.c_str(), port);
    return false;
  }
  return true;
}
