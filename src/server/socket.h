#ifndef NSFD_SERVER_SOCKETREADER_H_
#define NSFD_SERVER_SOCKETREADER_H_

#include <string>

#define BUFFER_SIZE 1024

class Socket
{
public:
  static char* read_raw_message(int sock_fd);
  static void write_and_close(int sock_fd, const std::string& msg);
};

#endif // NSFD_SERVER_SOCKETREADER_H_
