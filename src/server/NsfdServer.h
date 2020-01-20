
#ifndef NSFD_SERVER_COMMUNICATIONSESERVER_H_
#define NSFD_SERVER_COMMUNICATIONSESERVER_H_

#include "TcpServer.h"
#include "../watch/WatchTaskStorage.h"
#include "../watch/WatchTaskFactory.h"

class NsfdServer : public TcpServer
{
private:
  WatchTaskStorage* task_storage;
  WatchTaskFactory* task_factory;

public:
  NsfdServer(int port, ServerSupervisor*, WatchTaskStorage*, WatchTaskFactory*);
  ~NsfdServer() = default;

protected:
  void on_connection(int client_sock, const string& client_addr) override;
  void process_request(int client_sock, const string& client_addr, void* request);
};

#endif // NSFD_SERVER_COMMUNICATIONSESERVER_H_
