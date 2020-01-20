#include "WatchServiceRequest.h"

#include <sstream>

WatchServiceRequest::WatchServiceRequest(string& action, string& domain, string& port)
  : _action(action), _domain(domain), _port(port) {};

WatchServiceRequest::~WatchServiceRequest() = default;

bool WatchServiceRequest::is_create_request() const
{
  return _action == "create";
}

string WatchServiceRequest::domain() const
{
  return _domain;
}

string WatchServiceRequest::port() const
{
  return _port;
}

string WatchServiceRequest::to_string() const
{
  stringstream ss;
  ss << "action = " << this->_action << endl
     << "domain = " << this->_domain << endl
     << "port = " << this->_port << endl;
  return ss.str();
}
