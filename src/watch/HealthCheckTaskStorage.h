#ifndef NSFD_WATCH_WATCHTASKSTORAGE_H_
#define NSFD_WATCH_WATCHTASKSTORAGE_H_

#include <vector>
#include <functional>
#include "HealthCheckTask.h"

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

#endif // NSFD_WATCH_WATCHTASKSTORAGE_H_
