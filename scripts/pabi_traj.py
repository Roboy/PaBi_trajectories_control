#!/usr/bin/python

from std_srvs.srv import Trigger
from roboy_communication_middleware.msg import JointStatus
import os
import sys
import time

import rospy
import json


def record(req):

	angles = []
	times = []
	trajectory = {}

	def callback(data):
		angles.append(data.relAngles)
		times.append(time.time())

	start = time.time()
	rospy.Subscriber("/roboy/middleware/JointStatus", JointStatus, callback)

	try:
		while True:
			print('recording')
		    # store angles and time
	except KeyboardInterrupt:
		# dump the data
	    print('saving the data')
	    for i in range(len(times)):
	    	trajectory[start - times[i]] = angles
	    with open('trajectory', 'w') as outfile:
    		json.dump(trajectory, outfile)

	response = {}
	response["success"] = True
	response["message"] = "Recording PaBi trajectory was successful"
	return json.dumps(response)

def replay(req):
	response = {}
	response["success"] = True
	response["message"] = "Replaying PaBi trajectory was successful"
	return json.dumps(response)


if __name__ == '__main__':
	rospy.init_node('roboy_pabi_trajectories')

	rospy.Service('/roboy/control/pabi/trajectory/record', Trigger, record)
	rospy.Service('/roboy/control/pabi/trajectory/replay', Trigger, replay)
	
	print "/roboy/control/pabi/trajectory/ is ready"

	rospy.spin()