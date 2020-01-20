#ifndef NSFD_SERVER_API_WATCHSERVICEREQUEST_H_
#define NSFD_SERVER_API_WATCHSERVICEREQUEST_H_

#include <string>

using namespace std;

class WatchServiceRequest
{
private:
  const string _action;
  const string _domain;
  const string _port;

public:
  WatchServiceRequest(string& action, string& domain, string& port);
  ~WatchServiceRequest();

  bool is_create_request() const;
  string domain() const;
  string port() const;
  string to_string() const;
};

#endif // NSFD_SERVER_API_WATCHSERVICEREQUEST_H_
