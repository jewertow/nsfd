#include "server/NsfdServer.h"
#include "network/IcmpClient.h"
#include "network/TcpClient.h"
#include "watch/WatchServiceScheduler.h"
#include "server/MetricsServer.h"
#include <csignal>

void stop(int);

// FIXME: Wysłanie 1000 żądań powoduj błąd zbyt dużej liczby otwartych deskryptorów plików
// FIXME: IDE podswietla
ServerSupervisor* supervisor = new ServerSupervisor();

int main(int argc, char *argv[])
{
  signal(SIGINT, stop);
  // FIXME: tymczasowe obejście na problem rzucania broken pipe przez sockety, dlaczego dokładnie się to dzieje?
  signal(SIGPIPE, SIG_IGN);

  supervisor->enable_server();

  IcmpClient icmp_client;
  TcpClient tcp_client;

  WatchTaskFactory factory(&icmp_client, &tcp_client);
  WatchTaskStorage storage;

  NsfdServer srv(5000, supervisor, &storage, &factory);
  srv.run();

  MetricsServer metrics_server(5001, supervisor, &storage);
  metrics_server.run();

  WatchServiceScheduler scheduler(&storage, supervisor);
  scheduler.run();

  string cmd;
  do {
      cin >> cmd;
  } while (cmd != "stop");

  printf("Stopping server....\n");
  supervisor->disable_server();
  delete supervisor;

  exit(EXIT_SUCCESS);
}

void stop(int)
{
  printf("Stopping server...\n");
  supervisor->disable_server();
  delete supervisor;
  exit(EXIT_SUCCESS);
}
