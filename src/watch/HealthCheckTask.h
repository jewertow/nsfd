#ifndef NSFD_WATCH_WATCHTASK_H_
#define NSFD_WATCH_WATCHTASK_H_

#include <vector>
#include <mutex>
#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"

struct HealthCheckResult
{
  IcmpResult* icmp_result;
  TcpResult* tcp_result;
  long timestamp_ms;
};

class HealthCheckTask
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;
  const std::string _domain;
  const int port;
  const int results_size;

  std::mutex mutex;
  std::vector<HealthCheckResult*> results;

  // TODO: Zmienić nazwę na run()
  HealthCheckResult* _execute();
  void compact_results();

  void print_results();
  long now();

public:
  // TODO: Zmienić na const !!!
  HealthCheckTask(IcmpClient*, TcpClient*, std::string domain, int port, int results_size);
  ~HealthCheckTask();

  void execute();
  std::string domain() const;
  std::string results_string();
};

#endif // NSFD_WATCH_WATCHTASK_H_
