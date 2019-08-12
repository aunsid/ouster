#include <sensor_msgs/Imu.h>
#include <iostream>
#include <string>
#include <math.h>
#include <ros/ros.h>
#include "geometry_msgs/Vector3Stamped.h"
#include "std_msgs/Float64.h"
using namespace std;


class IMUdata
{      
   private:

            ros::NodeHandle nh;
            ros::Subscriber imu_sub;
            ros::Publisher IMU_error_pub;
            //geometry_msgs::Vector3 Imu_angluer_velocity;
            //geometry_msgs::Quaternion Imu_orientation;
            //geometry_msgs::Vector3 Imu_linear_acceleration;
            sensor_msgs::Imu convertIMU;
        
            std_msgs::Float64 imu_x;
            std_msgs::Float64 imu_y;
            std_msgs::Float64 imu_z;
            
            std_msgs::Float64 x_sum;
            std_msgs::Float64 y_sum;
            std_msgs::Float64 z_sum;
            
            std_msgs::Float64 x_error;
            std_msgs::Float64 y_error;
            std_msgs::Float64 z_error;
            geometry_msgs::Vector3 imu_error;
            geometry_msgs::Vector3 Imu_acc_error;
            int numbers=0;
            int loops=1;
   public:
        
            
            IMUdata() :
            nh("~"){
            
            string imu_topic ="/os1_node1/imu";       

            imu_sub = nh.subscribe(imu_topic.c_str(),1, &IMUdata::imuCallback, this);
            //IMU_conversion_pub = nh.advertise<sensor_msgs::Imu>("error",1);::imuCallback, this);
            IMU_error_pub = nh.advertise<geometry_msgs::Vector3>("error",1);
            
            }


   
   void imuCallback(const sensor_msgs::ImuPtr msg)
       {
               
               if(numbers ==2){
               cout<<msg->linear_acceleration.x<<endl<<-msg->linear_acceleration.y<<endl<<msg->linear_acceleration.z+9.80665<<endl;}
               if(numbers <= 500){
               numbers=numbers+1;
               //cout<<numbers<<endl;
               convertIMU.linear_acceleration.x = msg->linear_acceleration.x;
               convertIMU.linear_acceleration.y = -msg->linear_acceleration.y;
               convertIMU.linear_acceleration.z = msg->linear_acceleration.z+9.80665;
               
               Imu_acc_error.x=Imu_acc_error.x+convertIMU.linear_acceleration.x;
               Imu_acc_error.y=convertIMU.linear_acceleration.y+Imu_acc_error.y;
               Imu_acc_error.z=convertIMU.linear_acceleration.z+Imu_acc_error.z;
               //cout<<"number1"<<endl;
               /*x_sum =x_sum+x_error; 
               y_sum =y_sum+y_error; 
               z_sum =z_sum+z_error; */
               
               }
               
               if(numbers==501){
                     numbers=0;
                    
               imu_x.data = Imu_acc_error.x/(500*loops);
               imu_y.data = Imu_acc_error.y/(500*loops);
               imu_z.data = Imu_acc_error.z/(500*loops);
               cout<<"imu_x:  "<<imu_x.data<<endl<<"imu_y:  "<<imu_y.data<<endl<<"imu_z:  "<<imu_z.data<<endl;
               imu_error.x = imu_x.data;
               imu_error.y = imu_y.data;
               imu_error.z = imu_z.data;
               loops=loops+1;
               
               }  
               
               IMU_error_pub.publish(imu_error);
        }
               
};
       
       int main(int argc, char** argv)
{
    ros::init(argc, argv, "IMU_errormakeup");
    IMUdata Imu_error;
    ros::spin();

    return 0;
}

