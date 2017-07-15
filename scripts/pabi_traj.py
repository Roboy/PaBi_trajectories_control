#!/usr/bin/python

from std_srvs.srv import Trigger
from roboy_communication_middleware.msg import JointStatus
import os
import sys
import time

import rospy
import json

global angles
global times
global sub

def start_record(req):

	angles = []
	times = []
	trajectory = {}
	response = {}

	def callback(data):
		angles.append(data.relAngles)
		times.append(time.time())

	start = time.time()
	rospy.Subscriber("/roboy/middleware/JointStatus", JointStatus, callback)

	while True:
		print('recording')
		response["message"] = "Recording PaBi trajectory"

	response["success"] = True
	return json.dumps(response)

def stop_record(req):
	response = {}
	sub.unregister()
	# print('saving the data')
	# for i in range(len(times)):
	# 	trajectory[start - times[i]] = angles
	# with open('trajectory', 'w') as outfile:
	# 	json.dump(trajectory, outfile)

	response["success"] = True	
	response["message"] = "Recording PaBi trajectory was successful"

	angles = []
	times = []

	return json.dumps(response)

def replay(req):
	response = {}
	response["success"] = True
	response["message"] = "Replaying PaBi trajectory was successful"
	return json.dumps(response)


if __name__ == '__main__':
	angles = []
	times = []
	rospy.init_node('roboy_pabi_trajectories')

	rospy.Service('/roboy/control/pabi/trajectory/record/start', Trigger, start_record)
	rospy.Service('/roboy/control/pabi/trajectory/record/stop', Trigger, stop_record)
	rospy.Service('/roboy/control/pabi/trajectory/replay', Trigger, replay)
	
	print "/roboy/control/pabi/trajectory/ is ready"

	rospy.spin()