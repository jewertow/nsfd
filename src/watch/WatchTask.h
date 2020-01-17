#ifndef NSFD_WATCH_WATCHTASK_H_
#define NSFD_WATCH_WATCHTASK_H_

#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"

class WatchTask
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;

  std::string domain;
  int port;

public:
  WatchTask(IcmpClient*, TcpClient*, std::string domain, int port);

  void execute();
};

#endif // NSFD_WATCH_WATCHTASK_H_
