#include "TcpServer.h"

TcpServer::TcpServer(int port, ServerSupervisor* supervisor)
  : port(port), supervisor(supervisor) {}

TcpServer::~TcpServer() = default;

void TcpServer::run()
{
  thread t(&TcpServer::start, this);
  t.detach();
}

void TcpServer::start()
{
  printf("Server is running on %d\n", port);
  srv_sock_fd = SocketFactory::create_tcp_socket(port);
  sockaddr_in client_addr {};
  socklen_t sock_size = sizeof(sockaddr);
  while (this->supervisor->is_running())
  {
    process_connections(client_addr, sock_size);
  }
  printf("Server stopped\n");
  exit(0);
}

void TcpServer::process_connections(sockaddr_in& client_addr, socklen_t& sock_size)
{
  int client_sock_fd = accept(srv_sock_fd, (sockaddr*)& client_addr, &sock_size);
  if (client_sock_fd < 0)
  {
    printf("Cannot accept socket");
    return;
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
  printf("[DEBUG] Client ip %s. Socket desc: %d\n", client_ip, client_sock_fd);

  thread t(&TcpServer::on_connection, this, client_sock_fd, client_ip);
  t.detach();
}
