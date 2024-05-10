#include <iostream>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h" 
#include "rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

string name = "Frank";

string sign(string msg) {
    Document doc; 
    doc.Parse<0>(msg.c_str()).HasParseError();
    
    Document::AllocatorType& allocator = doc.GetAllocator();
    doc.AddMember("signature", 0, allocator);
    assert(doc.HasMember("signature"));

    const char *c = name.c_str();
    doc["signature"].SetString(rapidjson::StringRef(c));

    StringBuffer buffer;
    Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer); 
 
    return buffer.GetString();
}

void callback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("%s", msg->data.c_str());
    std_msgs::String s_msg;
    s_msg.data = sign(msg->data.c_str());

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("RaptorOUT", 10);
    pub.publish(s_msg);
}

int main(int argc, char **argv){
    ros::init(argc, argv, name);

    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("RaptorIN", 10, callback);
    ros::Publisher pub = nh.advertise<std_msgs::String>("RaptorOUT", 10);

    ros::spin();

    return 0;
}