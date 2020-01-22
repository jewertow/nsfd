#include "HealthCheckServer.h"
#include "Socket.h"
#include "api/WatchServiceRequest.h"
#include "api/WatchServiceRequestDeserializer.h"

HealthCheckServer::HealthCheckServer(int port, ServerSupervisor* supervisor, HealthCheckTaskStorage* storage, HealthCheckTaskFactory* factory)
  : TcpServer(port, supervisor), task_storage(storage), task_factory(factory) {}

void HealthCheckServer::on_connection(int client_sock, const string& client_addr)
{
  printf("[DEBUG] Client ip %s. Socket fd: %d\n", client_addr.c_str(), client_sock);

  char* raw_msg = Socket::read_raw_message(client_sock);
  WatchServiceRequest* request = WatchServiceRequestDeserializer::deserialize(raw_msg);

  printf("[DEBUG] Deserialized request:\n%s\n", request->to_string().c_str());

  if (request->is_create_request())
  {
    fprintf(stdout, "[INFO] Dodanie zadania dla domeny %s i portu %s\n",
            request->domain().c_str(), request->port().c_str());

    auto* watch_task = task_factory->create_watch_task(request);
    task_storage->add_task(watch_task);
  }

  Socket::write_and_close(client_sock, "OK");

  delete [] raw_msg;
  delete request;
}