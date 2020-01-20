#ifndef NSFD_SERVER_API_WATCHSERVICEREQUESTDESERIALIZER_H
#define NSFD_SERVER_API_WATCHSERVICEREQUESTDESERIALIZER_H

#include "WatchServiceRequest.h"

class WatchServiceRequestDeserializer
{
public:
  static WatchServiceRequest* deserialize(char* raw_message);
};

#endif // NSFD_SERVER_API_WATCHSERVICEREQUESTDESERIALIZER_H
