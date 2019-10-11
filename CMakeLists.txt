cmake_minimum_required(VERSION 2.8.3)
project(ger_drone_cbr)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
  std_srvs
  message_generation

  ##ardrone_autonomy

)


include_directories(${catkin_INCLUDE_DIRS})

add_library(funcoesPosition src/funcoesPosition.cpp)

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
 add_message_files(
	FILES
  	Position.msg
 )


## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
 )



################################################
## Declare ROS dynamic reconfigure parameters ##
################################################


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  #INCLUDE_DIRS include
  LIBRARIES ger_drone_cbr
  CATKIN_DEPENDS message_runtime roscpp rospy sensor_msgs std_msgs
  DEPENDS system_lib
)



###########
## Build ##
###########

add_executable(estrategia src/estrategia_main.cpp src/estrategia.cpp )
target_link_libraries(estrategia ${catkin_LIBRARIES} funcoesPosition)

add_executable(controleARDrone src/controle.cpp src/controleARDrone.cpp)
target_link_libraries(controleARDrone	 ${catkin_LIBRARIES})

add_executable(rota src/rota)
target_link_libraries(rota ${catkin_LIBRARIES})

add_executable(interface src/interface)
target_link_libraries(interface ${catkin_LIBRARIES})


add_dependencies(estrategia ${catkin_EXPORTED_TARGETS})
add_dependencies(controleARDrone ${catkin_EXPORTED_TARGETS})
add_dependencies(rota ${catkin_EXPORTED_TARGETS})
add_dependencies(interface ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############


#############
## Testing ##
#############
