#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>


using namespace std;
ros::Publisher pub1;


// Topic messages callback
//pose_callback
void UI(){

  char option;
  std_msgs::String msg;
  stringstream ss;


  cout << "Introduction to what the user can do here"<<endl;
  cout << "Choose one of the following options:"<<endl;
  cout << "Press w to go forward " <<endl;
  cout << "Press a to go right " <<endl;
  cout << "Press s to go back " <<endl;
  cout << "Press d to go to the left" <<endl;
  cout << "Press x to stop" <<endl;

  cin >> option;


  switch(option)
  {
    case ('w'):
    ss << "w";
    msg.data = ss.str();

    break;

    case ('a'):

    ss << "a";
    msg.data = ss.str();

    break;

    case ('s'):

    ss << "s";
    msg.data = ss.str();

    break;

    case ('d'):

    ss << "d";
    msg.data = ss.str();

    break;

    case ('x'):

    ss << "x";
    msg.data = ss.str();

    break;

  }
  pub1.publish(msg);
  ros::spinOnce();
  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle node;


  pub1 = node.advertise<std_msgs::String>("Commands", 1);

  // Enter a loop.
  while(ros::ok()){

    UI();

    ros::spinOnce();
  }
  return 0;
}
