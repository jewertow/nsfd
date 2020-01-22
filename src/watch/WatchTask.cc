#include "WatchTask.h"

#include <utility>
#include <sstream>
#include <sys/time.h>
#include "../util/CmdColor.h"

WatchTask::WatchTask(IcmpClient* icmp_client, TcpClient* tcp_client, std::string domain, int port, int results_size)
  : icmp_client(icmp_client), tcp_client(tcp_client), _domain(std::move(domain)), port(port), results_size(results_size) {}

WatchTask::~WatchTask()
{
  // TODO: mutex?
  for (auto& result : results)
  {
    delete result;
  }
  results.clear();
}

void WatchTask::execute()
{
  std::lock_guard<std::mutex> guard(mutex);
  printf("[DEBUG] WatchTask::execute %s\n", _domain.c_str());
  auto* result = this->_execute();
  results.push_back(result);
  // TODO: powinno to być konfigurowane jakąś zmienną globalną
//  this->print_results();
  this->compact_results();
}

std::string WatchTask::domain() const
{
  return _domain;
}

std::string WatchTask::results_string()
{
  std::lock_guard<std::mutex> guard(mutex);
  std::stringstream ss;
  for (auto& result : results)
  {
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
  if (result.length() > 0)
  {
    // remove first character "|"
    result.erase(0, 1);
  }

  return result;
}

WatchTaskResult* WatchTask::_execute()
{
  long timestamp = now();
  auto* watch_task_result = new WatchTaskResult{};
  watch_task_result->timestamp_ms = timestamp;

  auto* icmp_result = icmp_client->execute_request(this->_domain);
  watch_task_result->icmp_result = icmp_result;

  if (icmp_result == nullptr || !icmp_result->success)
  {
    fprintf(stdout, "%sICMP request to %s failed %s\n", RED, _domain.c_str(), RESET);
    // TODO: Wstawiać nullptr, ale odpowiednio go obslużyć potem
    watch_task_result->tcp_result = tcp_client->failed_result();
    return watch_task_result;
  }
  else
  {
    fprintf(stdout, "%sICMP request to %s succeeded! %s\n", GRN, _domain.c_str(), RESET);
  }

  auto* tcp_result = tcp_client->execute_request(_domain, port);

  if (!tcp_result->success)
  {
    fprintf(stdout, "%sTCP request to %s failed %s\n", RED, _domain.c_str(), RESET);
  }
  else
  {
    fprintf(stdout, "%sTCP request to %s succeeded! %s\n", GRN, _domain.c_str(), RESET);
  }

  watch_task_result->tcp_result = tcp_result;
  return watch_task_result;
}

void WatchTask::print_results()
{
  printf("--- %s ---\n", _domain.c_str());
  for (auto const& result : results)
  {
    if (result->icmp_result != nullptr)
    {
      printf("icmp success = %d\n", result->icmp_result->success);
      printf("icmp time = %Lf\n", result->icmp_result->rtt_ms);
    }
    if (result->tcp_result != nullptr)
    {
      printf("tcp success = %d\n", result->tcp_result->success);
      printf("tcp time = %Lf\n", result->tcp_result->time);
    }
    printf("Timestamp = %ld\n", result->timestamp_ms);
  }
}

long WatchTask::now()
{
  struct timeval timeval{};
  gettimeofday(&timeval, nullptr);
  long ms = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
  return ms;
}

void WatchTask::compact_results()
{
  if (results.size() > (unsigned int) this->results_size)
  {
    printf("Usuwanie ostatniego\n");
    auto* oldest = results.front();
    results.erase(results.begin());
    delete oldest;
  }
}
