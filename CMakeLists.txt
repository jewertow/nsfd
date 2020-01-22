cmake_minimum_required(VERSION 3.9)
project(Network-Service-Failure-Detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -Wall")

add_executable(nsfd-server
        src/nsfd.cc
        src/server/SocketFactory.cc
        src/server/SocketFactory.h
        src/server/HealthCheckServer.cc
        src/server/HealthCheckServer.h
		src/server/TcpServer.cc
        src/server/TcpServer.h
		src/server/Socket.cc
		src/server/Socket.h
		src/server/api/HealthCheckRequest.h
		src/server/api/HealthCheckRequest.cc
		src/server/ServerSupervisor.h
		src/server/ServerSupervisor.cc
		src/server/api/HealthCheckRequestDeserializer.h
		src/server/api/HealthCheckRequestDeserializer.cc
		src/network/IcmpClient.h
		src/network/IcmpClient.cc
		src/network/dns.h
		src/network/dns.cc
		src/network/TcpClient.h
		src/network/TcpClient.cc
		src/health/HealthCheckTask.h
		src/health/HealthCheckTask.cc
		src/health/HealthCheckTaskScheduler.h
		src/health/HealthCheckTaskScheduler.cc
		src/health/HealthCheckTaskStorage.h
		src/health/HealthCheckTaskStorage.cc
		src/health/HealthCheckTaskFactory.h
		src/health/HealthCheckTaskFactory.cc
		src/util/CmdColor.h
		src/server/MetricsServer.h
		src/server/MetricsServer.cc
		src/server/api/MetricsRequest.h
		src/server/api/MetricsRequestDeserializer.h
		src/server/api/MetricsRequestDeserializer.cc)
