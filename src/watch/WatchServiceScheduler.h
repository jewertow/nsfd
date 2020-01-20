#ifndef NSFD_WATCH_WATCHSERVICESCHEDULER_H_
#define NSFD_WATCH_WATCHSERVICESCHEDULER_H_

#include "WatchTaskStorage.h"
#include "../server/ServerSupervisor.h"

class WatchServiceScheduler
{
private:
  WatchTaskStorage* storage;
  ServerSupervisor* supervisor;
  void schedule();

public:
  WatchServiceScheduler(WatchTaskStorage*, ServerSupervisor*);
  ~WatchServiceScheduler() = default;
  void run();
};

#endif // NSFD_WATCH_WATCHSERVICESCHEDULER_H_
