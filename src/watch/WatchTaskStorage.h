#ifndef NSFD_WATCH_WATCHTASKSTORAGE_H_
#define NSFD_WATCH_WATCHTASKSTORAGE_H_

#include <vector>
#include <functional>
#include "WatchTask.h"

class WatchTaskStorage
{
private:
  std::vector<WatchTask*> tasks;

public:
  WatchTaskStorage() = default;
  void add_task(WatchTask*);
  void foreach(const std::function<void(WatchTask*)>& executor);
};

#endif // NSFD_WATCH_WATCHTASKSTORAGE_H_
