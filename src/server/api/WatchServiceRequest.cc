#include "WatchServiceRequest.h"

#include <sstream>

WatchServiceRequest::WatchServiceRequest(string& action, string& domain, string& port)
  : action(action), domain(domain), port(port) {};

WatchServiceRequest::~WatchServiceRequest() = default;

bool WatchServiceRequest::is_create_request()
{
  return action == "create";
}

string WatchServiceRequest::get_domain()
{
  return domain;
}

string WatchServiceRequest::get_port()
{
  return port;
}

string WatchServiceRequest::to_string()
{
  stringstream ss;
  ss  << "action = " << this->action << endl
      << "domain = " << this->domain << endl
      << "port = " << this->port << endl;
  return ss.str();
}
