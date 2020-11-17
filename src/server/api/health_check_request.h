#ifndef NSFD_SERVER_API_HEALTHCHECKREQUEST_H_
#define NSFD_SERVER_API_HEALTHCHECKREQUEST_H_

#include <string>

using namespace std;

class HealthCheckRequest
{
private:
  const string _action;
  const string _domain;
  const string _port;

public:
  HealthCheckRequest(string& action, string& domain, string& port);
  ~HealthCheckRequest() = default;

  bool is_create_request() const;
  string domain() const;
  string port() const;
  string to_string() const;
};

#endif // NSFD_SERVER_API_HEALTHCHECKREQUEST_H_
