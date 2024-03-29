#ifndef NSFD_HEALTH_HEALTHCHECKTASK_H_
#define NSFD_HEALTH_HEALTHCHECKTASK_H_

#include <vector>
#include <mutex>
#include "../network/icmp_client.h"
#include "../network/tcp_client.h"

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

  HealthCheckResult* execute_task();
  void compact_results();

  long now();

public:
  HealthCheckTask(IcmpClient*, TcpClient*, std::string domain, int port, int results_size);
  ~HealthCheckTask();

  void execute();
  std::string domain() const;
  std::string results_string();
};

#endif // NSFD_HEALTH_HEALTHCHECKTASK_H_
