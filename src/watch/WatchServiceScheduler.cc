#include "WatchServiceScheduler.h"
#include <thread>
#include <chrono>

WatchServiceScheduler::WatchServiceScheduler(WatchTaskStorage* storage, ServerSupervisor* supervisor)
  : storage(storage), supervisor(supervisor)
{
  std::thread t(&WatchServiceScheduler::schedule, this);
  t.detach();
  fprintf(stdout, "[INFO] WatchServiceScheduler started scheduling tasks\n");
}

void WatchServiceScheduler::schedule()
{
  while (supervisor->is_running())
  {
    execute();
    std::this_thread::sleep_for(std::chrono::seconds(15));
  }
}

void WatchServiceScheduler::execute()
{
  fprintf(stdout, "[INFO] scheduler executes tasks\n");
  storage->foreach([](WatchTask* task) -> void {
    // TODO: Wątek powinien byc dodawany do jakiejs mapy i ewentualnie ubijany
    std::thread t([&]() {
      task->execute();
    });
    t.detach();
  });
}
