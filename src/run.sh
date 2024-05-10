#! /bin/bash

g++ main.cpp -o main -I/opt/ros/noetic/include -L/opt/ros/noetic/lib -Wl,-rpath,/opt/ros/noetic/lib -lroscpp -lrosconsole -lroscpp_serialization

g++ messenger.cpp -o messenger -I/opt/ros/noetic/include -L/opt/ros/noetic/lib -Wl,-rpath,/opt/ros/noetic/lib -lroscpp -lrosconsole -lrostime -lroscpp_serialization
