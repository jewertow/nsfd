#include "WatchServiceRequestDeserializer.h"
#include <string.h>

WatchServiceRequest* WatchServiceRequestDeserializer::deserialize(char* raw_message)
{
  int i = 0;
  char* p = strtok(raw_message, ";");
  char* tokens[3];

  while (p != nullptr)
  {
    tokens[i++] = p;
    p = strtok (nullptr, ";");
  }

  string action(tokens[0]);
  string domain(tokens[1]);
  string port(tokens[2]);

  return new WatchServiceRequest(action, domain, port);
}