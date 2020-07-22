# NSFD

## What is NSFD?

NSFD (Network Service Failure Detector) is an academic project as a part of the course "Computer Networks".
It is a simple monitoring system that verifies the connection with a server (via the ICMP protocol)
and checks the availability of a network service running on a specific port by establishing a TCP connection.

Additionally NSFD measures and collects connection time for both protocols - ICMP and TCP. 

NSFD runs two TCP servers that allows for defining health checks and reading metrics.

## Building and running in docker container
```bash
docker build -t nsfd-server:latest .
docker run -it -p 8000:8000 -p 8001:8001 --name nsfd-server --privileged nsfd-server:latest
```

## Using with netcat as a client
Define scheduling health checks for facebook:
```bash
echo -n "create;facebook.com;80" | nc localhost 8000
```
First column is an action, second is a domain or IP address and third is a port of a network service to monitor.

Reading metrics:
```bash
echo -n "facebook.com" | nc localhost 8001
```
It will return results of health checks and connection time in the following format:
```
1;17.8211;1;18.2539;1595454676038
``` 
- 1st column is 1 or 0 - connection between services is possible or not.
- 2nd column is a connection time for ICMP protocol.
- 3rd column is 1 or 0 - defined network service is health or not.
- 4th column is a connection time for TCP protocol.

## Simulating ICMP connection failure
Run the command from the running container:
```bash
iptables -A OUTPUT -d facebook.com -j DROP
```
Now ICMP connections should fail. 
To restore the connection between servers execute command:
```bash
iptables -D OUTPUT -d facebook.com -j DROP
```

## GUI
NSFD has also dedicated desktop client available here: https://github.com/jewertow/nsfd-client

It provides GUI to configure health checks and display metrics on charts. 
