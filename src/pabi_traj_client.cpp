#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <roboy_communication_control/StartRecordTrajectory.h>
#include <roboy_communication_control/StopRecordTrajectory.h>
#include <roboy_communication_middleware/JointStatus.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <signal.h>

ros::ServiceClient stopClient;
ros::ServiceClient startClient;
roboy_communication_control::StopRecordTrajectory stopSrv;
roboy_communication_control::StartRecordTrajectory startSrv;

void interrupt_handler(int s){
   ROS_INFO("Recording the trajectory. Press Ctrl+C to stop");
   stopClient.call(stopSrv);
   auto traj = stopSrv.response.trajectory;

   // std::cout << traj << std::endl;
   std::ofstream o(startSrv.request.name+".json");
   o << std::setw(4) << traj << std::endl;
   exit(1); 

}

int main(int argc, char **argv) {
	ros::init(argc, argv, "pabi_traj_client");
	ros::NodeHandle n;
	startClient = n.serviceClient<roboy_communication_control::StartRecordTrajectory>("/roboy/control/pabi/trajectory/record/start");
	stopClient = n.serviceClient<roboy_communication_control::StopRecordTrajectory>("/roboy/control/pabi/trajectory/record/stop");

	std::cout << "Trajectory name: " << std::endl;
	std::cin >> startSrv.request.name; 
	

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = interrupt_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	if (startClient.call(startSrv))
	{
		ROS_INFO("Recording the trajectory. Press Ctrl+C to stop");
		while(1);
	}
	else
	{
		ROS_ERROR("Failed to call service /roboy/control/pabi/trajectory/record/start");
		return 1;
	}

	return 0;

}