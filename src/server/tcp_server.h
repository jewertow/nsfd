#ifndef NSFD_SERVER_TCPSERVER_H_
#define NSFD_SERVER_TCPSERVER_H_

#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server_supervisor.h"
#include "socket_factory.h"

using namespace std;

class TcpServer
{
protected:
  int port;
  ServerSupervisor* supervisor;
  // TODO: close socket and release fd in destructor
  int srv_sock_fd;

  virtual void on_connection(int client_sock_fd, const std::string& client_addr) = 0;

  TcpServer(int port, ServerSupervisor* supervisor);
  ~TcpServer() = default;

  void process_connections(sockaddr_in& client_addr, socklen_t& sock_size);
  void start();

public:
  void run();
};

#endif // NSFD_SERVER_TCPSERVER_H_
