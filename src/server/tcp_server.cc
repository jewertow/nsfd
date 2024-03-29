#include "tcp_server.h"

TcpServer::TcpServer(int port, ServerSupervisor* supervisor)
  : port(port), supervisor(supervisor) {}

void TcpServer::run()
{
  thread t(&TcpServer::start, this);
  t.detach();
}

void TcpServer::start()
{
  fprintf(stdout, "Server is running on port %d\n", port);
  srv_sock_fd = SocketFactory::create_tcp_socket(port);
  sockaddr_in client_addr {};
  socklen_t sock_size = sizeof(sockaddr);
  while (this->supervisor->is_running()) {
    process_connections(client_addr, sock_size);
  }
}

void TcpServer::process_connections(sockaddr_in& client_addr, socklen_t& sock_size)
{
  int client_sock_fd = accept(srv_sock_fd, (sockaddr*)& client_addr, &sock_size);
  if (client_sock_fd < 0) {
    fprintf(stdout, "Failed to accept connection on socket %d\n", client_sock_fd);
    return;
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

  thread t(&TcpServer::on_connection, this, client_sock_fd, client_ip);
  t.detach();
}
