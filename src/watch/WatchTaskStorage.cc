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
  printf("WatchTaskStorage::get_by_domain przed mutexem\n");
  std::lock_guard<std::mutex> guard(mutex);
  printf("WatchTaskStorage::get_by_domain za mutexem\n");
  for (auto const& task : tasks)
  {
    printf("WatchTaskStorage::get_by_domain w petli\n");
    printf("WatchTaskStorage::get_by_domain sprawdzanie %s i %s\n", task->domain().c_str(), domain.c_str());
    if (task->domain() == domain)
    {
      printf("WatchTaskStorage::get_by_domain znaleziony task\n");
      return task;
    }
  }
  printf("WatchTaskStorage::get_by_domain za petla\n");
  return nullptr;
}
