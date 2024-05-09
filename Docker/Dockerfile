FROM osrf/ros:noetic-desktop-full
ARG USER=user
ARG DEBIAN_FRONTEND=noninteractive
RUN sudo apt-get update --fix-missing
RUN apt-get install -y ros-noetic-moveit ros-noetic-ros-controllers ros-noetic-gazebo-ros-control 
RUN apt-get install -y ros-noetic-rosserial ros-noetic-rosserial-arduino 
RUN apt-get install -y ros-noetic-roboticsgroup-upatras-gazebo-plugins ros-noetic-actionlib-tools
RUN apt-get install -y rapidjson-dev python3-pip && rm -rf /var/lib/apt/lists/*
WORKDIR /home/${USER}