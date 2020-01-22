#include "HealthCheckTaskScheduler.h"
#include <thread>
#include <chrono>

HealthCheckTaskScheduler::HealthCheckTaskScheduler(HealthCheckTaskStorage* storage, ServerSupervisor* supervisor, int sleep_s)
  : storage(storage), supervisor(supervisor), sleep_s(sleep_s) {}

void HealthCheckTaskScheduler::run()
{
  std::thread t(&HealthCheckTaskScheduler::schedule, this);
  t.detach();
  fprintf(stdout, "[INFO] HealthCheckTaskScheduler started scheduling tasks\n");
}

void HealthCheckTaskScheduler::schedule()
{
  while (supervisor->is_running())
  {
    fprintf(stdout, "[INFO] executing tasks\n");
    storage->foreach([](HealthCheckTask* task) -> void {
        std::thread t(&HealthCheckTask::execute, task);
        t.detach();
    });
    std::this_thread::sleep_for(std::chrono::seconds(this->sleep_s));
  }
}
