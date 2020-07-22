FROM ubuntu:bionic

RUN apt-get update -y
RUN apt-get install -y g++ build-essential make cmake
RUN apt-get install -y iptables
RUN mkdir /opt/nsfd
RUN mkdir /opt/nsfd/build

COPY CMakeLists.txt /opt/nsfd
COPY src /opt/nsfd/src

RUN cmake -B/opt/nsfd/build -H/opt/nsfd
RUN make -C /opt/nsfd/build

CMD /opt/nsfd/build/nsfd-server
