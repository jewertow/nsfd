#include "health_check_task_storage.h"

HealthCheckTaskStorage::~HealthCheckTaskStorage()
{
  // TODO: mutex?
  for (auto& task : tasks)
  {
    delete task;
  }
  tasks.clear();
}

void HealthCheckTaskStorage::add_task(HealthCheckTask* watch_task)
{
  std::lock_guard<std::mutex> guard(mutex);
  tasks.push_back(watch_task);
}

void HealthCheckTaskStorage::foreach(const std::function<void (HealthCheckTask*)>& executor)
{
  std::lock_guard<std::mutex> guard(mutex);
  for (auto const& task : tasks)
  {
    printf("[DEBUG] Executing task for domain: %s\n", task->domain().c_str());
    executor(task);
  }
}

HealthCheckTask* HealthCheckTaskStorage::get_by_domain(std::string& domain)
{
  std::lock_guard<std::mutex> guard(mutex);
  for (auto const& task : tasks)
  {
    if (task->domain() == domain)
    {
      return task;
    }
  }
  return nullptr;
}
