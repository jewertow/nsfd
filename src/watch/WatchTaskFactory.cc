#include "WatchTaskFactory.h"

WatchTaskFactory::WatchTaskFactory(IcmpClient* icmp_client, TcpClient* tcp_client, int results_size)
  : icmp_client(icmp_client), tcp_client(tcp_client), results_size(results_size) {}

WatchTask* WatchTaskFactory::create_watch_task(WatchServiceRequest* request)
{
  return new WatchTask(icmp_client, tcp_client, request->domain(), std::stoi(request->port()), this->results_size);
}
