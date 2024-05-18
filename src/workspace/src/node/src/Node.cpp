#include "node/Node.h"

using namespace std;
using namespace rapidjson;

Node::Node(string _name)
{
    sub = nh.subscribe("RaptorIN", 10, &Node::callback, this);
    pub = nh.advertise<std_msgs::String>("RaptorOUT", 10);
    name = _name;
}

string Node::sign(string msg) 
{
    Document doc, d2; 
    bool error = doc.Parse<0>(msg.c_str()).HasParseError();
    if (error) {
        return "error";
    }
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

void Node::callback(const std_msgs::String::ConstPtr& msg) 
{
    // ROS_INFO("%s", msg->data.c_str());
    std_msgs::String signed_msg;
    signed_msg.data = sign(msg->data.c_str());
    pub.publish(signed_msg);
}