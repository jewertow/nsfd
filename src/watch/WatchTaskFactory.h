#ifndef NSFD_WATCH_WATCHTASKFACTORY_H_
#define NSFD_WATCH_WATCHTASKFACTORY_H_

#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"
#include "WatchTask.h"
#include "../server/api/WatchServiceRequest.h"

class WatchTaskFactory
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;
  int results_size;

public:
  WatchTaskFactory(IcmpClient*, TcpClient*, int);

  WatchTask* create_watch_task(WatchServiceRequest*);
};

#endif // NSFD_WATCH_WATCHTASKFACTORY_H_
