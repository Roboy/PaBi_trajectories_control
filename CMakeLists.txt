cmake_minimum_required(VERSION 2.8.3)
project(PaBi_trajectories_control)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  roboy_communication_control
  roboy_communication_middleware
  
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


catkin_package(
  CATKIN_DEPENDS roscpp roboy_communication_control roboy_communication_middleware
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(PaBi_trajectories_control src/pabi_traj.cpp)
add_dependencies(PaBi_trajectories_control ${catkin_EXPORTED_TARGETS} roboy_communication_control_gencpp roboy_communication_middleware_gencpp)
target_link_libraries(PaBi_trajectories_control
 ${catkin_LIBRARIES})

add_executable(record_PaBi src/pabi_traj_client.cpp)
add_dependencies(record_PaBi ${catkin_EXPORTED_TARGETS} roboy_communication_control_gencpp roboy_communication_middleware_gencpp)
target_link_libraries(record_PaBi
 ${catkin_LIBRARIES})
