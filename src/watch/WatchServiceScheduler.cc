#include "WatchServiceScheduler.h"
#include <thread>
#include <chrono>

WatchServiceScheduler::WatchServiceScheduler(WatchTaskStorage* storage, ServerSupervisor* supervisor, int sleep_s)
  : storage(storage), supervisor(supervisor), sleep_s(sleep_s) {}

void WatchServiceScheduler::run()
{
  std::thread t(&WatchServiceScheduler::schedule, this);
  t.detach();
  fprintf(stdout, "[INFO] WatchServiceScheduler started scheduling tasks\n");
}

void WatchServiceScheduler::schedule()
{
  while (supervisor->is_running())
  {
    fprintf(stdout, "[INFO] executing tasks\n");
    storage->foreach([](WatchTask* task) -> void {
        std::thread t(&WatchTask::execute, task);
        t.detach();
    });
    std::this_thread::sleep_for(std::chrono::seconds(this->sleep_s));
  }
}
