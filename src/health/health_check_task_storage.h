#ifndef NSFD_HEALTH_HEALTHCHECKTASKSTORAGE_H_
#define NSFD_HEALTH_HEALTHCHECKTASKSTORAGE_H_

#include <vector>
#include <functional>
#include "health_check_task.h"

class HealthCheckTaskStorage
{
private:
  std::vector<HealthCheckTask*> tasks;
  std::mutex mutex;

public:
  HealthCheckTaskStorage() = default;
  ~HealthCheckTaskStorage();

  void add_task(HealthCheckTask*);
  void foreach(const std::function<void(HealthCheckTask*)>& executor);
  HealthCheckTask* get_by_domain(std::string& domain);
};

#endif // NSFD_HEALTH_HEALTHCHECKTASKSTORAGE_H_
