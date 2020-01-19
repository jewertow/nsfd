#include "WatchTask.h"

#include <utility>
#include "../util/CmdColor.h"

WatchTask::WatchTask(IcmpClient* icmp_client, TcpClient* tcp_client, std::string domain, int port)
  : icmp_client(icmp_client), tcp_client(tcp_client), domain(std::move(domain)), port(port) {}

WatchTask::~WatchTask()
{
  for (auto& result : results)
  {
    delete result;
  }
  results.clear();
}

WatchTaskResult* WatchTask::_execute()
{
  auto* watch_task_result = new WatchTaskResult{};

  auto* icmp_result = icmp_client->execute_request(domain);
  watch_task_result->icmp_result = icmp_result;

  if (icmp_result == nullptr || !icmp_result->success)
  {
    fprintf(stdout, "%sICMP request to %s failed %s\n", RED, domain.c_str(), RESET);
    watch_task_result->tcp_result = nullptr;
    return watch_task_result;
  }
  else
  {
    fprintf(stdout, "%sICMP request to %s succeeded! %s\n", GRN, domain.c_str(), RESET);
  }

  auto* tcp_result = tcp_client->execute_request(domain, port);

  if (tcp_result == nullptr || !tcp_result->success)
  {
    fprintf(stdout, "%sTCP request to %s failed %s\n", RED, domain.c_str(), RESET);
  }
  else
  {
    fprintf(stdout, "%sTCP request to %s succeeded! %s\n", GRN, domain.c_str(), RESET);
  }

  watch_task_result->tcp_result = tcp_result;
  return watch_task_result;
}

void WatchTask::execute()
{
  printf("[DEBUG] WatchTask::execute %s\n", domain.c_str());
  auto* result = this->_execute();
  results.push_back(result);
  this->print_results();
}

void WatchTask::print_results()
{
  printf("--- %s ---\n", domain.c_str());
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
  }
}
