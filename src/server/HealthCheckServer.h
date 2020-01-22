#ifndef NSFD_SERVER_HEALTHCHECKSERVER_H_
#define NSFD_SERVER_HEALTHCHECKSERVER_H_

#include "TcpServer.h"
#include "../health/HealthCheckTaskStorage.h"
#include "../health/HealthCheckTaskFactory.h"

class HealthCheckServer : public TcpServer
{
private:
  HealthCheckTaskStorage* task_storage;
  HealthCheckTaskFactory* task_factory;

public:
  HealthCheckServer(int port, ServerSupervisor*, HealthCheckTaskStorage*, HealthCheckTaskFactory*);
  ~HealthCheckServer() = default;

protected:
  void on_connection(int client_sock, const string& client_addr) override;
};

#endif // NSFD_SERVER_HEALTHCHECKSERVER_H_
