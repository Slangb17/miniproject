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
unsigned int laser_ranges_size = 0;
string ans;
bool tooClose = false;


	geometry_msgs::Twist vel_msg;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
	ans = msg->data;
	ROS_INFO("I heard:[%s]", ans);
}

void move_based_on_message(){


	if(ans == "w") {

		vel_msg.linear.x = 0.5;
		vel_msg.angular.z = 0;


	}
	else if(ans == "a"){
		vel_msg.linear.x = 0;
		vel_msg.angular.z = 10;


	}
	else if (ans =="s"){
		vel_msg.linear.x = -0.5;
		vel_msg.angular.z = 0;


	}
	else if (ans== "d"){
		vel_msg.linear.x = 0;
		vel_msg.angular.z = -10;


	}
	else if (ans== "x"){
		vel_msg.linear.x = 0;
		vel_msg.angular.z = 0;


	}

}

void scan_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg)
{

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

	// The code inside the if-statement will only be run if a corner has NOT been found:
	if (tooClose != true)
	{
		ROS_INFO("waiting for message");
		move_based_on_message();
	}

	// This is used to determine when a corner has been found:
	// The idea is to use this to initialize the goal marking process:
	if (laser_ranges_min<0.6)
	{
		vel_msg.linear.x = 0;
		vel_msg.angular.z = 0;
		tooClose==true;
		ROS_INFO("STOPPED! TOO CLOSE TO WALL!!!");

	}




	pub.publish(vel_msg);
}




int main(int argc, char**argv){
	ros::init(argc, argv, "publish_velocity");


	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("Commands", 1000, chatterCallback);
	ros::Subscriber laser_scan = n.subscribe("/scan", 1000, scan_callback);

	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1000);
	ros::Rate rate(10);
	while(ros::ok()){
move_based_on_message();
		ros::spinOnce();
		rate.sleep();

	}
	return 0;
}
