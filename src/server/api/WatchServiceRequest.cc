#include "WatchServiceRequest.h"

#include <sstream>

WatchServiceRequest::WatchServiceRequest(string& action, string& domain, string& port)
  : action(action), domain(domain), port(port) {};

WatchServiceRequest::~WatchServiceRequest() = default;

string WatchServiceRequest::to_string()
{
  stringstream ss;
  ss  << "action = " << this->action << endl
      << "domain = " << this->domain << endl
      << "port = " << this->port << endl;
  return ss.str();
}
