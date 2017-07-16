#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <roboy_communication_control/StartRecordTrajectory.h>
#include <roboy_communication_control/StopRecordTrajectory.h>

#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

class TrajectoriesManager
{
	private:
	bool recording;
	json trajectory;

	public:
		TrajectoriesManager(){
			this->recording = false;
			// this->trajectory = "";
		}
		~TrajectoriesManager();



	bool startRecord(roboy_communication_control::StartRecordTrajectory::Request &req,
	                     roboy_communication_control::StartRecordTrajectory::Response &res) {

	      this->recording = true;
	      ROS_INFO_STREAM("recording start");

	      // json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
	      int j = 0;
	      while(this->recording)
	      {
			this->trajectory[j] = "fuck";
			j++;
	      }
	      ROS_INFO_STREAM("recording done");
	      res.success = true;
	      return true;
	  }

	bool stopRecord(roboy_communication_control::StopRecordTrajectory::Request &req,
	                     roboy_communication_control::StopRecordTrajectory::Response &res) {
	      this->recording = false;
	      res.trajectory = this->trajectory.dump();
	      this->trajectory = "";
	      return true;
	  }
	
};

int main(int argc, char **argv) {

	ros::init(argc, argv, "talker");

	ros::NodeHandle n;

	auto tm = new TrajectoriesManager();

	auto startRecordSrv = n.advertiseService("/roboy/control/pabi/trajectory/record/start", &TrajectoriesManager::startRecord, tm);
	auto stopRecordSrv = n.advertiseService("/roboy/control/pabi/trajectory/record/stop", &TrajectoriesManager::stopRecord, tm);

	// ros::Subscriber jointStatusSub = n.subscribe("/roboy/middleware/JointStatus", 1000, &MyoMotor::moveMotorCallback, &myo_control);

	ros::AsyncSpinner spinner (0);
	while (ros::ok())
	{
	 spinner.start();
	}
	spinner.stop();
}

