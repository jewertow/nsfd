#ifndef NSFD_WATCH_WATCHSERVICESCHEDULER_H_
#define NSFD_WATCH_WATCHSERVICESCHEDULER_H_

#include "WatchTaskStorage.h"
#include "../server/ServerSupervisor.h"

class WatchServiceScheduler
{
private:
  WatchTaskStorage* storage;
  ServerSupervisor* supervisor;
  int sleep_s;

  void schedule();

public:
  WatchServiceScheduler(WatchTaskStorage*, ServerSupervisor*, int sleep_s);
  ~WatchServiceScheduler() = default;
  void run();
};

#endif // NSFD_WATCH_WATCHSERVICESCHEDULER_H_
