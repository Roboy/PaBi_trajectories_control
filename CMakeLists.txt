cmake_minimum_required(VERSION 2.8.3)
project(PaBi_trajectories_control)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
  message_generation
  
)

#  services in the 'srv' folder
# add_service_files(
#   FILES
#   Movement.srv
#   Yaw.srv

# )

# ## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   EmotionMsg.msg
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS rospy std_msgs message_generation 
)


#############
## Install ##
#############
## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
 install(PROGRAMS
   srs/move.py
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

## Mark other files for installation (e.g. launch and bag files, etc.)
 install(DIRECTORY launch config
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

