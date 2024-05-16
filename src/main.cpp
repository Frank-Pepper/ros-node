#include <iostream>
#include <chrono>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

class Node{
    private:
    ros::NodeHandle nh;
    ros::Subscriber sub;
    ros::Publisher pub;
    string name;
    public:
    Node(string _name){

        sub = nh.subscribe("RaptorIN", 10, &Node::callback, this);
        pub = nh.advertise<std_msgs::String>("RaptorOUT", 10);

        name = _name;
    }

    string sign(string msg) {
        Document doc, d2; 
        doc.Parse<0>(msg.c_str()).HasParseError();
        int64_t timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

        Document::AllocatorType& al2 = d2.GetAllocator();
        Value json_objects(kObjectType);
        json_objects.AddMember("name", StringRef(name.c_str()), al2);
        json_objects.AddMember("timestamp", timestamp, al2);
        json_objects.AddMember("data", "Something else", al2);

        Value& array = doc["recruits"];
        assert(array.IsArray());
        array.PushBack(json_objects, doc.GetAllocator());

        StringBuffer buffer;
        Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer); 
    
        return buffer.GetString();
    }

        void callback(const std_msgs::String::ConstPtr& msg) {
        ROS_INFO("%s", msg->data.c_str());
        std_msgs::String s_msg;
        s_msg.data = sign(msg->data.c_str());
        pub.publish(s_msg);
    }
};

int main(int argc, char **argv){
    string name = "Frank";
    ros::init(argc, argv, name);

    Node node = Node(name);
    ros::spin();
    return 0;
}