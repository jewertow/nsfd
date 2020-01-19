#include "WatchTaskStorage.h"

WatchTaskStorage::~WatchTaskStorage()
{
  for (auto& task : tasks)
  {
    delete task;
  }
  tasks.clear();
}

void WatchTaskStorage::add_task(WatchTask* watch_task)
{
  tasks.push_back(watch_task);
}

void WatchTaskStorage::foreach(const std::function<void (WatchTask *)>& executor)
{
  for (auto const& task : tasks)
  {
    printf("[DEBUG] Executing task for domain: %s\n", task->domain.c_str());
    executor(task);
  }
}
