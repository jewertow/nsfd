
#ifndef NSFD_SERVER_COMMUNICATIONSESERVER_H_
#define NSFD_SERVER_COMMUNICATIONSESERVER_H_

#include "TcpServer.h"

class NsfdServer : public TcpServer
{
public:
  NsfdServer(int port, ServerSupervisor* supervisor);
  ~NsfdServer();

protected:
  void on_connection(int client_sock, const string& client_addr) override;
  void process_request(int client_sock, const string& client_addr, void* request);
};

#endif // NSFD_SERVER_COMMUNICATIONSESERVER_H_
