#include "NsfdServer.h"
#include "Socket.h"
#include "api/WatchServiceRequest.h"
#include "api/WatchServiceRequestDeserializer.h"

NsfdServer::NsfdServer(int port, ServerSupervisor* supervisor, WatchTaskStorage* storage, WatchTaskFactory* factory)
  : TcpServer(port, supervisor), task_storage(storage), task_factory(factory) {}

void NsfdServer::on_connection(int client_sock, const string& client_addr)
{
  printf("[DEBUG] Client ip %s. Socket fd: %d\n", client_addr.c_str(), client_sock);

  char* raw_msg = Socket::read_raw_message(client_sock);
  WatchServiceRequest* request = WatchServiceRequestDeserializer::deserialize(raw_msg);
  delete [] raw_msg;

  printf("[DEBUG] Deserialized request:\n%s\n", request->to_string().c_str());

  process_request(client_sock, client_addr, request);
}

void NsfdServer::process_request(int client_sock, const string& client_addr, void* request)
{
  auto* watch_service_req = static_cast<WatchServiceRequest*>(request);
  printf("[DEBUG] Processing deserialized request:\n%s\n", watch_service_req->to_string().c_str());

  if (watch_service_req->is_create_request())
  {
    fprintf(stdout, "[INFO] Dodanie zadania dla domeny %s i portu %s\n",
            watch_service_req->domain().c_str(), watch_service_req->port().c_str());

    auto* watch_task = task_factory->create_watch_task(watch_service_req);
    task_storage->add_task(watch_task);
  }

  delete watch_service_req;
  Socket::write_and_close(client_sock, "OK");
}