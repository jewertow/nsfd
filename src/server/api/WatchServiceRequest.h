#ifndef NSFD_SERVER_API_WATCHSERVICEREQUEST_H_
#define NSFD_SERVER_API_WATCHSERVICEREQUEST_H_

#include <string>

using namespace std;

class WatchServiceRequest
{
private:
  string action;
  string domain;
  string port;

public:
  WatchServiceRequest(string& action, string& domain, string& port);
  ~WatchServiceRequest();
  string to_string();
};

#endif // NSFD_SERVER_API_WATCHSERVICEREQUEST_H_
