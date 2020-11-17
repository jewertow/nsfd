#include "health_check_request.h"

#include <sstream>

HealthCheckRequest::HealthCheckRequest(string& action, string& domain, string& port)
  : _action(action), _domain(domain), _port(port) {};

bool HealthCheckRequest::is_create_request() const
{
  return _action == "create";
}

string HealthCheckRequest::domain() const
{
  return _domain;
}

string HealthCheckRequest::port() const
{
  return _port;
}

string HealthCheckRequest::to_string() const
{
  stringstream ss;
  ss << "action = " << this->_action << endl
     << "domain = " << this->_domain << endl
     << "port = " << this->_port << endl;
  return ss.str();
}
