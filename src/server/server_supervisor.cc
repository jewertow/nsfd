#include "server_supervisor.h"

bool ServerSupervisor::is_running()
{
  return this->running;
}

void ServerSupervisor::enable_server()
{
  this->running = true;
}

void ServerSupervisor::disable_server()
{
  this->running = false;
}
