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

  // TODO: Dostęp do wyników powinien być chroniony mutexem
  std::vector<WatchTaskResult*> results;

  // TODO: Zmienić nazwę na run()
  WatchTaskResult* _execute();

public:
  // TODO: Zmienić na const !!!
  std::string domain;
  int port;
  WatchTask(IcmpClient*, TcpClient*, std::string domain, int port);
  ~WatchTask();
  // TODO: Dodać dekonstruktor zwalniający wektor

  void execute();
  void print_results();
};

#endif // NSFD_WATCH_WATCHTASK_H_
