#ifndef NSFD_SERVER_SOCKETFACTORY_H_
#define NSFD_SERVER_SOCKETFACTORY_H_

#include <netinet/in.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <string>


#define QUEUE_SIZE 128

// TODO: Usunąć klasę i zostawić same funkcje!!

class SocketFactory
{
  // TODO: Wykorzystac do tworzenia raw socketów
  static int create_socket(int port);

public:
  static int create_tcp_socket(int port);
  static int create_tcp_client_socket(int port);
};


#endif // NSFD_SERVER_SOCKETFACTORY_H_
