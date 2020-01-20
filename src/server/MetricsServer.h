#ifndef NSFD_SERVER_METRICSSERVER_H_
#define NSFD_SERVER_METRICSSERVER_H_

#include "TcpServer.h"
#include "../watch/WatchTaskStorage.h"

class MetricsServer : public TcpServer
{
private:
  WatchTaskStorage* task_storage;

protected:
  void on_connection(int client_sock, const string& client_addr) override;
  void process_request(int client_sock, const string& client_addr, void* request);

public:
  MetricsServer(int port, ServerSupervisor*, WatchTaskStorage*);
  ~MetricsServer() = default;
};

#endif // NSFD_SERVER_METRICSSERVER_H_
