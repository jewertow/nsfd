#ifndef NSFD_WATCH_WATCHTASK_H_
#define NSFD_WATCH_WATCHTASK_H_

#include <vector>
#include <mutex>
#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"

struct WatchTaskResult
{
  IcmpResult* icmp_result;
  TcpResult* tcp_result;
  long timestamp_ms;
};

class WatchTask
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;
  const std::string _domain;
  const int port;
  std::mutex mutex;

  // TODO: Dostęp do wyników powinien być chroniony mutexem
  std::vector<WatchTaskResult*> results;

  // TODO: Zmienić nazwę na run()
  WatchTaskResult* _execute();
  void print_results();
  long now();

public:
  // TODO: Zmienić na const !!!
  WatchTask(IcmpClient*, TcpClient*, std::string domain, int port);
  ~WatchTask();

  void execute();
  std::string domain() const;
  std::string results_string();
};

#endif // NSFD_WATCH_WATCHTASK_H_
