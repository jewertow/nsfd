#ifndef NSFD_HEALTH_HEALTHCHECKTASKSCHEDULER_H_
#define NSFD_HEALTH_HEALTHCHECKTASKSCHEDULER_H_

#include "HealthCheckTaskStorage.h"
#include "../server/ServerSupervisor.h"

class HealthCheckTaskScheduler
{
private:
  HealthCheckTaskStorage* storage;
  ServerSupervisor* supervisor;
  int sleep_s;

  void schedule();

public:
  HealthCheckTaskScheduler(HealthCheckTaskStorage*, ServerSupervisor*, int sleep_s);
  ~HealthCheckTaskScheduler() = default;
  void run();
};

#endif // NSFD_HEALTH_HEALTHCHECKTASKSCHEDULER_H_
