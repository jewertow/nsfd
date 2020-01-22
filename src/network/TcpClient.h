#ifndef NSFD_NETWORK_TCPCLIENT_H_
#define NSFD_NETWORK_TCPCLIENT_H_

#include <string>

struct TcpResult
{
  bool success;
  long double time;
};

// TODO: Metody zmienic na static
class TcpClient
{
public:
  TcpResult* execute_request(const std::string& addr, int port);
  TcpResult* failed_result();
};

#endif // NSFD_NETWORK_TCPCLIENT_H_
