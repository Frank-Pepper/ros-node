#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h" 


using namespace rapidjson;
using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "messenger");

    FILE* fp = fopen("message.json", "rb"); 

    if (!fp) { 
        std::cerr << "Error: unable to open file" << std::endl; 
        return 1; 
    } 

    char readBuffer[65536]; 
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); 

    rapidjson::Document doc; 
    doc.ParseStream(is); 
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    ros::NodeHandle n;

    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("RaptorIN", 10);

    ros::Rate loop_rate(10);

    int count = 0;
    while (count < 10)
    {
        std_msgs::String msg;
        msg.data = buffer.GetString();

        ROS_INFO("%s", msg.data.c_str());

        chatter_pub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }
    ros::spin();

    return 0;
}