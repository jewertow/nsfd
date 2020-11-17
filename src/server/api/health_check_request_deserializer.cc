#include "health_check_request_deserializer.h"
#include <string.h>

HealthCheckRequest* HealthCheckRequestDeserializer::deserialize(char* raw_message)
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

  return new HealthCheckRequest(action, domain, port);
}