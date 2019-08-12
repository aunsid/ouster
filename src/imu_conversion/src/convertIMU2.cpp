#include <sensor_msgs/Imu.h>
#include <iostream>
#include <string>
#include <ros/ros.h>

#include "tf/tf.h"
#include "tf/transform_listener.h"
#include "geometry_msgs/Vector3Stamped.h"
#include "std_msgs/Float32.h"
using namespace std;


class convert
{
    private:

        ros::NodeHandle nh;
        ros::Subscriber imu_sub;
        ros::Publisher IMU_conversion_pub;
        geometry_msgs::Vector3 Imu_angluer_velocity;
        geometry_msgs::Quaternion Imu_orientation;
        geometry_msgs::Vector3 Imu_linear_acceleration;
        
        /*struct sensor_msgs::Imu IMUdata
        {
         geometry_msgs::Vector3 Imu_angluer_velocity;
         geometry_msgs::Quaternion Imu_orientation;
         geometry_msgs::Vector3 Imu_linear_acceleration;   
        };*/
        //float imu_x;
        //float imu_y;
        //float imu_z;
       // sensor_msgs::geometry_msgs::Vector3

    public:
        convert() :
        nh("~"){

        string imu_topic = "os1_node1/imu";
        //string imu_frame = "os1_imu";

        imu_sub = nh.subscribe(imu_topic.c_str(),1, &convert::imuCallback, this);
        IMU_conversion_pub = nh.advertise<sensor_msgs::Imu>("convertimu_data",1);

    }


     void imuCallback(const sensor_msgs::ImuConstPtr& msg)
     {  



        sensor_msgs::Imu convertIMU;

        tf::Vector3 v_angluer_velocity;
        tf::Quaternion v_orientation;
        tf::Vector3 v_linear_acceleration;

        tf::Vector3 vs_angluer_velocity;
        tf::Quaternion vs_orientation;
        tf::Vector3 vs_linear_acceleration;
      //  convertIMU = msg;

        Imu_angluer_velocity = msg->angular_velocity;
        Imu_orientation = msg->orientation;
        Imu_linear_acceleration = msg->linear_acceleration;

       tf::vector3MsgToTF (Imu_angluer_velocity,v_angluer_velocity);
       tf::vector3MsgToTF (Imu_linear_acceleration,v_linear_acceleration);
       tf::quaternionMsgToTF(Imu_orientation, v_orientation);  

        vs_angluer_velocity.x = v_angluer_velocity.x;
        vs_angluer_velocity.y = -v_angluer_velocity.y;
        vs_angluer_velocity.z = -v_angluer_velocity.z;s
        vs_orientation.y = -v_orientation.x;
        vs_orientation.z = -v_orientation.y;
        vs_orientation.w = -v_orientation.z;
        vs_orientation.x = v_orientation.w;
        vs_linear_acceleration.x = v_linear_acceleration.x;
        vs_linear_acceleration.y = -v_linear_acceleration.y;
        vs_linear_acceleration.z = -v_linear_acceleration.z;

       tf::vector3TFToMsg(vs_angluer_velocity,Imu_angluer_velocity); 
       tf::vector3TFToMsg(vs_linear_acceleration,Imu_linear_acceleration); 
       tf::quaternionTFToMsg(vs_orientation,Imu_orientation); 

         convertIMU.orientation = Imu_orientation;
         convertIMU.angluer_velocity = Imu_angluer_velocity;
         convertIMU.linear_acceleration = Imu_linear_acceleration;


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
