cmake_minimum_required(VERSION 3.9)
project(Network-Service-Failure-Detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -Wall")

add_executable(nsfd-server
        src/nsfd.cc
        src/server/SocketFactory.cc
        src/server/SocketFactory.h
		src/server/NsfdServer.cc
		src/server/NsfdServer.h
		src/server/TcpServer.cc
        src/server/TcpServer.h
		src/server/Socket.cc
		src/server/Socket.h
		src/server/api/WatchServiceRequest.h
		src/server/api/WatchServiceRequest.cc
		src/server/ServerSupervisor.h
		src/server/ServerSupervisor.cc
		src/server/api/WatchServiceRequestDeserializer.h
		src/server/api/WatchServiceRequestDeserializer.cc
		src/network/IcmpClient.h
		src/network/IcmpClient.cc
		src/network/dns.h
		src/network/dns.cc
		src/network/TcpClient.h
		src/network/TcpClient.cc
		src/watch/WatchTask.h
		src/watch/WatchTask.cc
		src/watch/WatchServiceScheduler.h
		src/watch/WatchServiceScheduler.cc
		src/watch/WatchTaskStorage.h
		src/watch/WatchTaskStorage.cc
		src/watch/WatchTaskFactory.h
		src/watch/WatchTaskFactory.cc
		src/util/CmdColor.h)
