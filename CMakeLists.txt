cmake_minimum_required(VERSION 3.9)
project(Network-Service-Failure-Detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -Wall")

add_executable(nsfd-server
        src/nsfd.cc
		src/server/socket_factory.cc
		src/server/socket_factory.h
		src/server/health_check_server.cc
		src/server/health_check_server.h
		src/server/tcp_server.cc
		src/server/tcp_server.h
		src/server/socket.cc
		src/server/socket.h
		src/server/api/health_check_request.h
		src/server/api/health_check_request.cc
		src/server/server_supervisor.h
		src/server/server_supervisor.cc
		src/server/api/health_check_request_deserializer.h
		src/server/api/health_check_request_deserializer.cc
		src/network/icmp_client.h
		src/network/icmp_client.cc
		src/network/dns.h
		src/network/dns.cc
		src/network/tcp_client.h
		src/network/tcp_client.cc
		src/health/health_check_task.h
		src/health/health_check_task.cc
		src/health/health_check_task_scheduler.h
		src/health/health_check_task_scheduler.cc
		src/health/health_check_task_storage.h
		src/health/health_check_task_storage.cc
		src/health/health_check_task_factory.h
		src/health/health_check_task_factory.cc
		src/util/cmd_colors.h
		src/server/metrics_server.h
		src/server/metrics_server.cc
		src/server/api/metrics_request.h
		src/server/api/metrics_request_deserializer.h
		src/server/api/metrics_request_deserializer.cc)
