#ifndef NSFD_NETWORK_TCPCLIENT_H_
#define NSFD_NETWORK_TCPCLIENT_H_

#include <string>

class TcpClient
{
public:
  bool execute_request(std::string &addr, int port);
};

#endif // NSFD_NETWORK_TCPCLIENT_H_
