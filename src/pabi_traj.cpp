#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <roboy_communication_control/StartRecordTrajectory.h>
#include <roboy_communication_control/StopRecordTrajectory.h>
#include <roboy_communication_middleware/JointStatus.h>

#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

class TrajectoriesManager
{
	private:
	bool recording;
	json trajectory;
	ros::Time startTime;


	public:
		TrajectoriesManager(){
			this->recording = false;
		}
		~TrajectoriesManager();

	void jointStatusListener(const roboy_communication_middleware::JointStatus::ConstPtr& msg)
	{
		ros::Rate loop_rate(200);
		if (this->recording)
		{
			auto angles = msg->relAngles;
			std::vector<double> v;
			v.push_back((ros::Time::now()-startTime).toSec());
			for(auto &a: angles)
			{
				v.push_back(a/4096.0 * 360.0 - 180.0);
			}
			trajectory["setpoints"].push_back(v);
			loop_rate.sleep();
		}
		
	}

	bool startRecord(roboy_communication_control::StartRecordTrajectory::Request &req,
	                     roboy_communication_control::StartRecordTrajectory::Response &res) {

	      this->recording = true;
	      this->startTime = ros::Time::now();
	      trajectory["name"] = req.name;
	      ROS_INFO_STREAM("recording started");
	      res.success = true;
	      return true;
	  }

	bool stopRecord(roboy_communication_control::StopRecordTrajectory::Request &req,
	                     roboy_communication_control::StopRecordTrajectory::Response &res) {
	      this->recording = false;
	      ROS_INFO_STREAM("recording done");
	      res.trajectory = this->trajectory.dump();
	      this->trajectory = json::value_t::object;
	      return true;
	  }
	
};

int main(int argc, char **argv) {

	ros::init(argc, argv, "talker");

	ros::NodeHandle n;

	auto tm = new TrajectoriesManager();

	auto startRecordSrv = n.advertiseService("/roboy/control/pabi/trajectory/record/start", &TrajectoriesManager::startRecord, tm);
	auto stopRecordSrv = n.advertiseService("/roboy/control/pabi/trajectory/record/stop", &TrajectoriesManager::stopRecord, tm);

	ros::Subscriber jointStatusSub = n.subscribe("/roboy/middleware/JointStatus", 1000, &TrajectoriesManager::jointStatusListener, tm);

	ros::AsyncSpinner spinner (0);
	while (ros::ok())
	{
	 spinner.start();
	}
	spinner.stop();
}

