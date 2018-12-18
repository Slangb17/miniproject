#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <sstream>
#include "turtlesim/Pose.h"
#include <iostream>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

ros::Publisher pub;
ros::Subscriber pose_subscriber;
ros::Subscriber UI;

sensor_msgs::LaserScan laser_msg;
std::vector<float> laser_ranges;

using namespace std;
unsigned short laser_ranges_size = 0;
string ans;
bool tooClose = false;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
	ans = msg->data;
	ROS_INFO("I heard:[%s]", ans);
}

void move_based_on_message(){
	geometry_msgs::Twist msg;

	if(ans == "w") {
		msg.linear.x = 0.5;
		msg.angular.z = 0;
		pub.publish(msg);

	}
	else if(ans == "a"){
		msg.linear.x = 0;
		msg.angular.z = 10;
		pub.publish(msg);

	}
	else if (ans =="s"){
		msg.linear.x = -0.5;
		msg.angular.z = 0;
		pub.publish(msg);

	}
	else if (ans== "d"){
		msg.linear.x = 0;
		msg.angular.z = -10;
		pub.publish(msg);

	}
	else if (ans== "x"){
		msg.linear.x = 0;
		msg.angular.z = 0;
		pub.publish(msg);

	}
}

void scan_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg)
{
	geometry_msgs::Twist msg;
	// The msg is stored in laser_msg:
	laser_msg = *scan_msg;

	// The ranges from the sensor readings in laser_ranges:
	laser_ranges = laser_msg.ranges;
	laser_ranges_size = laser_ranges.size();

	// The min and max sensor readings are found:
	float laser_ranges_min = 5.0f;
	float laser_ranges_max = 0.0f;

	for (int i = 0; i < laser_ranges_size; i++)
	{
		if(!std::isnan(laser_ranges[i])){
			if (laser_ranges[i] < laser_ranges_min)
			{
				laser_ranges_min = laser_ranges[i];
			}
			if (laser_ranges[i] > laser_ranges_max)
			{
				laser_ranges_max = laser_ranges[i];
			}
		}
	}

	// The sensor ranges are divided into three sections: left, center, and right
	float laser_ranges_left = 0.0f;
	float laser_ranges_center = 0.0f;
	float laser_ranges_right = 0.0f;

	for (int i = 0; i < laser_ranges_size; i++)
	{
		if (!std::isnan(laser_ranges[i]))
		{
			if (i >= 0 && i < 213)
			{
				laser_ranges_right += laser_ranges[i];
			}
			else if (i < 426)
			{
				laser_ranges_center += laser_ranges[i];
			}
			else
			{
				laser_ranges_left += laser_ranges[i];
			}
		}
	}

	// The mean of the sections are found:
	laser_ranges_right /= (laser_ranges_size / 3.0);
	laser_ranges_center /= (laser_ranges_size / 3.0);
	laser_ranges_left /= (laser_ranges_size / 3.0);

	// The sections are printed in the terminal:

	//  ROS_INFO("Left: %f, center: %f, right: %f", laser_ranges_left,
	//  laser_ranges_center, laser_ranges_right);

	// The code inside the if-statement will only be run if a corner has NOT been found:
	if (tooClose != true)
	{
		ROS_INFO("waiting for message");
				msg.linear.x = 0.5;
		move_based_on_message();
	}

	// This is used to determine when a corner has been found:
	// The idea is to use this to initialize the goal marking process:
	if (laser_ranges_right<0.6 || laser_ranges_left<0.6 || laser_ranges_center<0.6)
	{
		msg.linear.x = 0;
		msg.angular.z = 0;
		tooClose==true;
		ROS_INFO("STOPPED! TOO CLOSE TO WALL!!!");

	}




	pub.publish(msg);
}




int main(int argc, char**argv){
	ros::init(argc, argv, "publish_velocity");


	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("Commands", 1000, chatterCallback);
	ros::Subscriber laser_scan = n.subscribe("/scan", 1000, scan_callback);
	//pose_subscriber = n.subscribe("/turtle1/pose",10 , poseCallback);
	//pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1000);
	ros::Rate rate(10);
	while(ros::ok()){

		ros::spinOnce();
		rate.sleep();

	}
	return 0;
}
