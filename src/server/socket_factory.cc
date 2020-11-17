#include <arpa/inet.h>
#include <cstring>
#include "socket_factory.h"


int SocketFactory::create_socket(int port) {
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0)
  {
    printf("Socket error\n");
    exit(EXIT_FAILURE);
  }

  sockaddr_in sock_addr_in{};
  sock_addr_in.sin_family = AF_INET;
  sock_addr_in.sin_port = htons(port);
  // TODO: jesli wsytąpi problem to ustawic tu inet_addr("127.0.0.1")
  sock_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

  int time = 1;
  // TODO: Przeanalizować czy dobrze działa
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &time, sizeof(time));

  if (bind(sock_fd, (struct sockaddr *) &sock_addr_in, sizeof(sockaddr_in)) < 0)
  {
    perror("Binding error");
    exit(EXIT_FAILURE);
  }

  return sock_fd;
}

int SocketFactory::create_tcp_socket(int port)
{
  int socket = create_socket(port);
  if (listen(socket, QUEUE_SIZE) < 0)
  {
    perror("Listen error");
    exit(EXIT_FAILURE);
  }
  return socket;
}
