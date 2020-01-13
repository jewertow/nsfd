#include "NsfdServer.h"
#include "Socket.h"
#include "api/WatchServiceRequest.h"
#include "api/WatchServiceRequestDeserializer.h"

NsfdServer::NsfdServer(int port, ServerSupervisor* supervisor)
  : TcpServer(port, supervisor) {}

NsfdServer::~NsfdServer() {}

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
  delete watch_service_req;

  Socket::write_and_close(client_sock, "OK");
}