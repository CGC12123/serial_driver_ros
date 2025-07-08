cmake_minimum_required(VERSION 3.0.2)
project(serial_driver)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  serial
)

###################################
## catkin specific configuration ##
###################################

catkin_package(

)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(serial_driver_node
  src/serial_main.cpp
  src/serial_driver.cpp
)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

#############
## Testing ##
#############

