# Train Scheduling App

## Introduction

Train Scheduling is a networking project I had to complete for Networking course. This is a connection-oriented client-server app where multiple clients can connect
to server and perform commands concurrently. For serving multiple clients at the same time, I designed a multithreaded server.
## Implementation details

A client can connect to a server via port 2049 on localhost. Client can send the following commands:
- ```C++ UPDATE -train <id> -delay <minutes> -fromStation <station_name> -toStation <station_name>```
- ```C++ ARRIVALS -stationPS <station_name> -stationD <station_name> -fromHour HH:MM -toHour HH:MM```
- ```C++ DEPARTURES -stationPS <station_name> -stationD <station_name> -fromHour HH:MM -toHour HH:MM```

The commands are implemented with the help of Command Design Pattern and requests sent by a client are handled with the help of a Command Queue Pattern. 
## Requirements
Qt 6.4.1 version is required for running the project. Make sure you modify the CMAKE_PREFIX_PATH from CMakeLists.txt with the path from your machine which points
to Qt cmake folder.

```C++
set(CMAKE_PREFIX_PATH /PATH/TO/Qt/6.4.1/COMPILER/lib/cmake)
```
