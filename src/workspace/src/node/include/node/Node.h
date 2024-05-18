#ifndef NODE_H_
#define NODE_H_

#include <chrono>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

class Node 
{
    private:
        std::string name;
        ros::NodeHandle nh;
        ros::Subscriber sub;
        ros::Publisher pub;
    public:
        Node(std::string _name);
        void callback(const std_msgs::String::ConstPtr& msg);
        std::string sign(std::string msg);
};
#endif // NODE_H_