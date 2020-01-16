#include "WatchTaskStorage.h"

void WatchTaskStorage::add_task(WatchTask* watch_task)
{
  tasks.push_back(watch_task);
}

void WatchTaskStorage::foreach(const std::function<void (WatchTask *)>& executor)
{
  for (auto const& task : tasks)
  {
    executor(task);
  }
}
