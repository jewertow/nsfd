#ifndef NSFD_SERVER_API_METRICSREQUESTDESERIALIZER_H_
#define NSFD_SERVER_API_METRICSREQUESTDESERIALIZER_H_

#include "MetricsRequest.h"

class MetricsRequestDeserializer
{
public:
  static MetricsRequest* deserialize(char* raw_message);
};

#endif // NSFD_SERVER_API_METRICSREQUESTDESERIALIZER_H_