#include "metrics_server.h"
#include "socket.h"
#include "api/metrics_request_deserializer.h"

MetricsServer::MetricsServer(int port, ServerSupervisor* supervisor, HealthCheckTaskStorage* storage)
  : TcpServer(port, supervisor), task_storage(storage) {}

void MetricsServer::on_connection(int client_sock, const string &client_addr)
{
  printf("[DEBUG] MetricsServer received request from ip %s. Socket fd: %d\n", client_addr.c_str(), client_sock);

  char* raw_msg = Socket::read_raw_message(client_sock);
  auto* request = MetricsRequestDeserializer::deserialize(raw_msg);

  printf("[DEBUG] Deserialized MetricsRequest:\n%s\n", request->domain.c_str());

  auto* task = task_storage->get_by_domain(request->domain);
  if (task != nullptr)
  {
    fprintf(stdout, "[INFO] Task %s results as string: %s\n", task->domain().c_str(), task->results_string().c_str());
    Socket::write_and_close(client_sock, task->results_string());
  }
  else
  {
    fprintf(stdout, "[INFO] Task %s not found\n", request->domain.c_str());
    Socket::write_and_close(client_sock, "NULL");
  }

  delete [] raw_msg;
  delete request;
}