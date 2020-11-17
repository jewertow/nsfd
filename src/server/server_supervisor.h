#ifndef NSFD_SERVER_SERVERSUPERVISOR_H_
#define NSFD_SERVER_SERVERSUPERVISOR_H_

class ServerSupervisor
{
private:
  bool running = false;

public:
  bool is_running();
  void enable_server();
  void disable_server();
};

#endif // NSFD_SERVER_SERVERSUPERVISOR_H_
