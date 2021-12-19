#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "math.h"

/*
double x, y, z, w;
double yaw, pitch, roll;
float accx, accy, accz; */

sensor_msgs::Imu imu, imu2;
std::string frameId;


void CallbackIMU(const sensor_msgs::Imu & Imu_message);
void CallbackGPS_IMU(const sensor_msgs::Imu & Imu_gps);


void CallbackIMU(const sensor_msgs::Imu & Imu_message){

        
	    ROS_INFO("got spatial subcribed");
		imu.header.stamp = ros::Time::now();
	    imu.header.frame_id = frameId;

		imu.linear_acceleration.x = Imu_message.linear_acceleration.x;
		imu.linear_acceleration.y = -(Imu_message.linear_acceleration.y);
    	imu.linear_acceleration.z = -(Imu_message.linear_acceleration.z);

		imu.angular_velocity.x = Imu_message.angular_velocity.x;
		imu.angular_velocity.y = Imu_message.angular_velocity.y;
		imu.angular_velocity.z = -(Imu_message.angular_velocity.z);

		imu.orientation.x = Imu_message.orientation.x; 
		imu.orientation.y = Imu_message.orientation.y;
		imu.orientation.z = Imu_message.orientation.z;
		imu.orientation.w = Imu_message.orientation.w;

		imu.linear_acceleration_covariance = Imu_message.linear_acceleration_covariance;
		imu.angular_velocity_covariance = Imu_message.angular_velocity_covariance;
		imu.orientation_covariance = Imu_message.orientation_covariance;


		imu2.orientation.x = Imu_message.orientation.x;
    	imu2.orientation.y = Imu_message.orientation.y;
    	imu2.orientation.z = Imu_message.orientation.z;
    	imu2.orientation.w = Imu_message.orientation.w;
	
    	/*if(isnan(x)){
    		printf("Error: IMU orientation in nan\n");
			pub.publish(imu);
			
    	} */
		if (isnan(imu2.orientation.x)){
			printf("Error: IMU orientation in nan\n");
		}
    /*	
    	yaw = atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
    	pitch = asin(-2.0 * (x*z - w*y));
    	roll = atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);

		printf("Yaw: %f Pitch: %f Roll: %f \n\n", yaw, pitch, roll);
		printf("accx:  %f accy: %f accz: %f \n\n", accx,accy,accz); */

}

void CallbackGPS_IMU(const sensor_msgs::Imu & Imu_gps){

        ROS_INFO("got GPS IMU");
		imu2.header.stamp = ros::Time::now();
	    imu.header.frame_id = frameId;

		imu2.linear_acceleration.x = -(Imu_gps.linear_acceleration.x);
		imu2.linear_acceleration.y = Imu_gps.linear_acceleration.y;
    	imu2.linear_acceleration.z = -Imu_gps.linear_acceleration.z;

		imu2.angular_velocity.x = (Imu_gps.angular_velocity.x);
		imu2.angular_velocity.y = Imu_gps.angular_velocity.y;
		imu2.angular_velocity.z = -(Imu_gps.angular_velocity.z * M_PI) / 180;

		imu2.orientation.x = Imu_gps.orientation.x; 
		imu2.orientation.y = Imu_gps.orientation.y;
		imu2.orientation.z = Imu_gps.orientation.z;
		imu2.orientation.w = Imu_gps.orientation.w;

		imu2.linear_acceleration_covariance = Imu_gps.linear_acceleration_covariance;
		imu2.angular_velocity_covariance = Imu_gps.angular_velocity_covariance;
		imu2.orientation_covariance = Imu_gps.orientation_covariance;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "imus_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/imu/data", 1000, CallbackIMU);
  ros::Subscriber sub_gps = n.subscribe("ublox/imu_meas", 1000, CallbackGPS_IMU);
  ros::Publisher pub = n.advertise<sensor_msgs::Imu>("imu/filtered",100);
  ros::Publisher pub_gps = n.advertise<sensor_msgs::Imu>("imu/filtered_gps",100);
  ros::Rate loop_rate(250);
  
  while (ros::ok())
  {
 	pub.publish(imu);
	pub_gps.publish(imu2);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
