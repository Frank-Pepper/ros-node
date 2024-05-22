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
    bool parsingError = doc.Parse<0>(msg.c_str()).HasParseError();

    int64_t timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    Document::AllocatorType& al2 = d2.GetAllocator();
    Value json_objects(kObjectType);
    json_objects.AddMember("name", StringRef(name.c_str()), al2);
    json_objects.AddMember("timestamp", timestamp, al2);
    json_objects.AddMember("data", "Something else", al2);
    
    bool error = 0;
    if (parsingError) 
    {
        json_objects["data"].SetString("json parsing error");
        error = 1;
    } 
    else if (!(doc.HasMember("recruits") && doc["recruits"].IsArray())) 
    {
        json_objects["data"].SetString("error: document has no member 'recruits' which is array type");
        error = 1;
    }

    if (error) 
    {
        doc.SetObject();
        Value errorMsg(kArrayType);
        errorMsg.PushBack(json_objects, doc.GetAllocator());
        doc.AddMember("recruits", errorMsg, doc.GetAllocator());
    }
    else
    {
        Value& array = doc["recruits"];
        array.PushBack(json_objects, doc.GetAllocator());
    }

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