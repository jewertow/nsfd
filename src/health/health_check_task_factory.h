#ifndef NSFD_HEALTH_HEALTHCHECKTASKFACTORY_H_
#define NSFD_HEALTH_HEALTHCHECKTASKFACTORY_H_

#include "../network/icmp_client.h"
#include "../network/tcp_client.h"
#include "health_check_task.h"
#include "../server/api/health_check_request.h"

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
