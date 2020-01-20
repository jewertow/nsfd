#include "WatchTaskStorage.h"

WatchTaskStorage::~WatchTaskStorage()
{
  // TODO: mutex?
  for (auto& task : tasks)
  {
    delete task;
  }
  tasks.clear();
}

void WatchTaskStorage::add_task(WatchTask* watch_task)
{
  std::lock_guard<std::mutex> guard(mutex);
  tasks.push_back(watch_task);
}

void WatchTaskStorage::foreach(const std::function<void (WatchTask*)>& executor)
{
  std::lock_guard<std::mutex> guard(mutex);
  for (auto const& task : tasks)
  {
    printf("[DEBUG] Executing task for domain: %s\n", task->domain().c_str());
    executor(task);
  }
}

WatchTask* WatchTaskStorage::get_by_domain(std::string& domain)
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
