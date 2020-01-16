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
  void execute();

public:
  WatchServiceScheduler(WatchTaskStorage*, ServerSupervisor*);
};

#endif // NSFD_WATCH_WATCHSERVICESCHEDULER_H_
