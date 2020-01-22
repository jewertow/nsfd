#ifndef NSFD_HEALTH_HEALTHCHECKTASKFACTORY_H_
#define NSFD_HEALTH_HEALTHCHECKTASKFACTORY_H_

#include "../network/IcmpClient.h"
#include "../network/TcpClient.h"
#include "HealthCheckTask.h"
#include "../server/api/HealthCheckRequest.h"

class HealthCheckTaskFactory
{
private:
  IcmpClient* icmp_client;
  TcpClient* tcp_client;
  int results_size;

public:
  HealthCheckTaskFactory(IcmpClient*, TcpClient*, int);

  HealthCheckTask* create_watch_task(HealthCheckRequest*);
};

#endif // NSFD_HEALTH_HEALTHCHECKTASKFACTORY_H_
