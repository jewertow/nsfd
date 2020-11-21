#include "socket.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

char* Socket::read_raw_message(int sock_fd)
{
  char* raw_msg = new char[BUFFER_SIZE];
  memset(raw_msg, 0, BUFFER_SIZE);
  if (read(sock_fd, raw_msg, BUFFER_SIZE) < 0) {
    fprintf(stdout, "Failed to read data from socket %d\n", sock_fd);
  }
  return raw_msg;
}

void Socket::write_and_close(int sock_fd, const std::string& msg)
{
  if (write(sock_fd, msg.c_str(), msg.length()) < 0) {
    fprintf(stdout, "Failed to write response to socket %d\n", sock_fd);
  }

  if (close(sock_fd)) {
    fprintf(stdout, "Failed to close socket %d\n", sock_fd);
  }
}
