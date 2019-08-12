#include <sensor_msgs/Imu.h>
#include <iostream>
#include <string>
#include <ros/ros.h>
#include "geometry_msgs/Vector3Stamped.h"
using namespace std;


class convert
{
    private:

        ros::NodeHandle nh;
        ros::Subscriber imu_sub;
        ros::Publisher IMU_conversion_pub;
        //geometry_msgs::Vector3 Imu_angluer_velocity;
        //geometry_msgs::Quaternion Imu_orientation;
        //geometry_msgs::Vector3 Imu_linear_acceleration;
        sensor_msgs::Imu convertIMU;
        //float imu_x;
        //float imu_y;
        //float imu_z;

    public:
        convert() :
        nh("~"){

        string imu_topic = "os1_node1/imu";
        //string imu_frame = "os1_imu";

        imu_sub = nh.subscribe(imu_topic.c_str(),1, &convert::imuCallback, this);
        IMU_conversion_pub = nh.advertise<sensor_msgs::Imu>("convertimu_data",1);

    }


     void imuCallback(const sensor_msgs::ImuPtr msg)
     {  


        convertIMU = msg;
        convertIMU.angular_velocity.x = msg->angular_velocity.x;
        convertIMU.angular_velocity.y = -msg->angular_velocity.y;
        convertIMU.angular_velocity.z = -msg->angular_velocity.z;
        convertIMU.orientation.y = -msg->orientation.x;
        convertIMU.orientation.z = -msg->orientation.y;
        convertIMU.orientation.w = -msg->orientation.z;
        convertIMU.orientation.x = msg->orientation.w;
        convertIMU.linear_acceleration.x = msg->linear_acceleration.x;
        convertIMU.linear_acceleration.y = -msg->linear_acceleration.y;
        convertIMU.linear_acceleration.z = -msg->linear_acceleration.z;



        IMU_conversion_pub.publish(convertIMU);

     }

};



int main(int argc, char** argv)
{
    ros::init(argc, argv, "IMU_conversion");
    convert detector;
    ros::spin();

    return 0;
}
