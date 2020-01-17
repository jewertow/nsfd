#include "WatchTaskFactory.h"

WatchTaskFactory::WatchTaskFactory(IcmpClient* icmp_client, TcpClient* tcp_client)
  : icmp_client(icmp_client), tcp_client(tcp_client) {}

WatchTask* WatchTaskFactory::create_watch_task(WatchServiceRequest* request)
{
  return new WatchTask(icmp_client, tcp_client, request->get_domain(), std::stoi(request->get_port()));
}
