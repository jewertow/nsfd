#ifndef NSFD_WATCH_WATCHTASKSTORAGE_H_
#define NSFD_WATCH_WATCHTASKSTORAGE_H_

#include <vector>
#include <functional>
#include "WatchTask.h"

class WatchTaskStorage
{
private:
  std::vector<WatchTask*> tasks;
  std::mutex mutex;

public:
  WatchTaskStorage() = default;
  ~WatchTaskStorage();

  void add_task(WatchTask*);
  void foreach(const std::function<void(WatchTask*)>& executor);
  WatchTask* get_by_domain(std::string& domain);
};

#endif // NSFD_WATCH_WATCHTASKSTORAGE_H_
