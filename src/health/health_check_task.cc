#include "health_check_task.h"

#include <utility>
#include <sstream>
#include <sys/time.h>
#include "../util/cmd_colors.h"

HealthCheckTask::HealthCheckTask(IcmpClient* icmp_client, TcpClient* tcp_client, std::string domain, int port, int results_size)
  : icmp_client(icmp_client), tcp_client(tcp_client), _domain(std::move(domain)), port(port), results_size(results_size) {}

HealthCheckTask::~HealthCheckTask()
{
  for (auto& result : results) {
    delete result;
  }
  results.clear();
}

void HealthCheckTask::execute()
{
  std::lock_guard<std::mutex> guard(mutex);
  auto* result = this->execute_task();
  results.push_back(result);
  this->compact_results();
}

std::string HealthCheckTask::domain() const
{
  return _domain;
}

std::string HealthCheckTask::results_string()
{
  std::lock_guard<std::mutex> guard(mutex);
  std::stringstream ss;
  for (auto& result : results) {
    ss  << "|"
        << result->icmp_result->success
        << ";"
        << result->icmp_result->rtt_ms
        << ";"
        << result->tcp_result->success
        << ";"
        << result->tcp_result->time
        << ";"
        << result->timestamp_ms;
  }

  auto result = ss.str();
  if (result.length() > 0) {
    // remove first character "|"
    result.erase(0, 1);
  }

  return result;
}

HealthCheckResult* HealthCheckTask::execute_task()
{
  long timestamp = now();
  auto* watch_task_result = new HealthCheckResult{};
  watch_task_result->timestamp_ms = timestamp;

  auto* icmp_result = icmp_client->execute_request(this->_domain);
  watch_task_result->icmp_result = icmp_result;

  if (icmp_result == nullptr || !icmp_result->success) {
    printf("%sICMP request to %s failed%s\n", RED, _domain.c_str(), RESET);
    watch_task_result->tcp_result = tcp_client->failed_result();
    return watch_task_result;
  } else {
    printf("%sICMP request to %s succeeded%s\n", GRN, _domain.c_str(), RESET);
  }

  auto* tcp_result = tcp_client->execute_request(_domain, port);
  if (!tcp_result->success) {
    printf("%sTCP request to %s failed%s\n", RED, _domain.c_str(), RESET);
  } else {
    printf("%sTCP request to %s succeeded%s\n", GRN, _domain.c_str(), RESET);
  }

  watch_task_result->tcp_result = tcp_result;
  return watch_task_result;
}

long HealthCheckTask::now()
{
  struct timeval timeval{};
  gettimeofday(&timeval, nullptr);
  long ms = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
  return ms;
}

void HealthCheckTask::compact_results()
{
  if (results.size() > (unsigned int) this->results_size) {
    auto* oldest = results.front();
    results.erase(results.begin());
    delete oldest;
  }
}
