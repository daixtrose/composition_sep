cmake_minimum_required(VERSION 3.5)

## cmake -DCMAKE_PREFIX_PATH=/opt/ros/eloquent ..

project(composition)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

################################################################################
# Add libraries from subfolders

add_subdirectory(client_component)
add_subdirectory(listener_component)
add_subdirectory(server_component)
add_subdirectory(talker_component)

find_package(ament_cmake REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(manual_composition
  src/manual_composition.cpp)

target_link_libraries(manual_composition
#  talker_component
#  listener_component
#  server_component
  client_component)

ament_target_dependencies(manual_composition
  "rclcpp")
