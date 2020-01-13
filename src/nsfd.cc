#include "server/NsfdServer.h"
#include "network/IcmpClient.h"
#include "network/TcpClient.h"
#include <csignal>

void stop(int);

// TODO: Wysłanie 1000 żądań powoduj błąd zbyt dużej liczby otwartych deskryptorów plików
// FIXME: IDE podswietla
ServerSupervisor* supervisor = new ServerSupervisor();

int main(int argc, char *argv[]) {
  signal(SIGINT, stop);
  // FIXME: tymczasowe obejście na problem rzucania broken pipe przez sockety, dlaczego dokładnie się to dzieje?
  signal(SIGPIPE, SIG_IGN);

  supervisor->enable_server();
  NsfdServer srv(5000, supervisor);

  string google = "google.com";
  IcmpClient icmp_client;
  bool icmp_health_check = icmp_client.execute_request(google);
  TcpClient tcp_client;
  bool tcp_health_check = tcp_client.execute_request(google, 80);

  if (icmp_health_check && tcp_health_check)
    printf("Google is working\n");
  else
    printf("Google is down\n");

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