#ifndef NSFD_WATCH_WATCHTASK_H_
#define NSFD_WATCH_WATCHTASK_H_

#include <vector>
#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"

struct WatchTaskResult
{
  IcmpResult* icmp_result;
  TcpResult* tcp_result;
};

class WatchTask
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;

  std::vector<WatchTaskResult*> results;

  WatchTaskResult* _execute();

public:
  // TODO: Change to const !!!
  std::string domain;
  int port;
  WatchTask(IcmpClient*, TcpClient*, std::string domain, int port);

  void execute();
  void print_results();
};

#endif // NSFD_WATCH_WATCHTASK_H_
