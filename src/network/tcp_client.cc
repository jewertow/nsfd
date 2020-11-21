#include "tcp_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

TcpResult* TcpClient::execute_request(const std::string& addr, int port)
{
  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    fprintf(stderr, "Failed to create a client socket [socket fd=%d]\n", socket_fd);
    return TcpClient::failed_result();
  }

  struct timeval timeout{};
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (char*) &timeout, sizeof(timeout));

  struct hostent* host_entity;
  if ((host_entity = gethostbyname(addr.c_str())) == nullptr) {
    fprintf(stderr, "Failed to resolve dns address %s\n", addr.c_str());
    return TcpClient::failed_result();
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  memcpy(&server_addr.sin_addr.s_addr, host_entity->h_addr, host_entity->h_length);

  struct timespec time_start{};
  clock_gettime(CLOCK_MONOTONIC, &time_start);

  if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
    fprintf(stderr, "Failed to connect to the server %s:%i.\n", addr.c_str(), port);
    return TcpClient::failed_result();
  }

  struct timespec time_end{};
  clock_gettime(CLOCK_MONOTONIC, &time_end);

  if (close(socket_fd) > 0) {
    fprintf(stderr, "Failed to close socket %d.\n", socket_fd);
  }

  double time_ns = ((double) (time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
  long double time_s = (long double) (time_end.tv_sec - time_start.tv_sec) * 1000.0 + time_ns;

  return new TcpResult{
    .success = true,
    .time = time_s
  };
}

TcpResult* TcpClient::failed_result()
{
  return new TcpResult{
    .success = false,
    .time = -1
  };
}
