#include "server/health_check_server.h"
#include "network/icmp_client.h"
#include "network/tcp_client.h"
#include "health/health_check_task_scheduler.h"
#include "server/metrics_server.h"
#include "health/health_check_task_factory.h"
#include <csignal>

void stop(int);

// FIXME: Wysłanie 1000 żądań powoduj błąd zbyt dużej liczby otwartych deskryptorów plików
// FIXME: IDE podswietla
ServerSupervisor* supervisor = new ServerSupervisor();

int main(int argc, char *argv[])
{
  signal(SIGINT, stop);
  // FIXME: tymczasowe obejście na problem rzucania broken pipe przez sockety
  signal(SIGPIPE, SIG_IGN);

  const int health_check_retention = 30;
  const int health_check_sleep_s = 5;

  IcmpClient icmp_client;
  TcpClient tcp_client;

  HealthCheckTaskStorage storage;
  HealthCheckTaskFactory factory(&icmp_client, &tcp_client, health_check_retention);

  supervisor->enable_server();

  HealthCheckServer health_check_server(8000, supervisor, &storage, &factory);
  health_check_server.run();

  MetricsServer metrics_server(8001, supervisor, &storage);
  metrics_server.run();

  HealthCheckTaskScheduler scheduler(&storage, supervisor, health_check_sleep_s);
  scheduler.run();

  string cmd;
  do {
      cin >> cmd;
  } while (cmd != "stop");

  printf("Stop servers...\n");
  supervisor->disable_server();
  delete supervisor;
  exit(EXIT_SUCCESS);
}

void stop(int)
{
  printf("Stop server...\n");
  supervisor->disable_server();
  delete supervisor;
  exit(EXIT_SUCCESS);
}
