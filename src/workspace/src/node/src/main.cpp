#include "ros/ros.h"
#include "node/Node.h"

int main(int argc, char **argv) 
{
    std::string name = "Frank";
    ros::init(argc, argv, name);
    Node node = Node(name);
    ros::spin();
    return 0;
}