#include "metrics_request_deserializer.h"

MetricsRequest* MetricsRequestDeserializer::deserialize(char *raw_message)
{
  return new MetricsRequest{
    .domain = std::string(raw_message)
  };
}