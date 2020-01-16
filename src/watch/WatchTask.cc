#include "WatchTask.h"
#include "../util/CmdColor.h"

WatchTask::WatchTask(IcmpClient* icmp_client, TcpClient* tcp_client, std::string& domain, int port)
  : icmp_client(icmp_client), tcp_client(tcp_client), domain(domain), port(port) {}

void WatchTask::execute()
{
  bool icmp_connected = icmp_client->execute_request(domain);
  if (!icmp_connected) {
    fprintf(stdout, "%sICMP request to %s failed %s\n", RED, domain.c_str(), RESET);
    return;
  }
  fprintf(stdout, "%sICMP request to %s succeeded! %s\n", GRN, domain.c_str(), RESET);

  bool tcp_connected = tcp_client->execute_request(domain, port);
  if (!tcp_connected) {
    fprintf(stdout, "%sTCP request to %s failed %s\n", RED, domain.c_str(), RESET);
    return;
  }
  fprintf(stdout, "%sTCP request to %s succeeded! %s\n", GRN, domain.c_str(), RESET);
}
