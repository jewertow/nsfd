#ifndef NSFD_SERVER_TCPSERVER_H_
#define NSFD_SERVER_TCPSERVER_H_

#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ServerSupervisor.h"
#include "SocketFactory.h"

using namespace std;

class TcpServer
{
protected:
  int port;
  ServerSupervisor* supervisor;
//  thread* srv_thread;
  int srv_sock_fd;

  virtual void on_connection(int client_sock_fd, const std::string& client_addr) = 0;

  TcpServer(int port, ServerSupervisor* supervisor);
  ~TcpServer();

  void process_connections(sockaddr_in& client_addr, socklen_t& sock_size);
  void start();

};


#endif // NSFD_SERVER_TCPSERVER_H_
