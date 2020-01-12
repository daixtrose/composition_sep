cmake_minimum_required(VERSION 3.5)

## cmake -DCMAKE_PREFIX_PATH=/opt/ros/eloquent ..

project(composition)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

################################################################################
# Add libraries from subfolders

add_subdirectory(listener_component)
add_subdirectory(client_component)
add_subdirectory(server_component)
add_subdirectory(talker_component)




################################################################################
# search packages

find_package(ament_cmake REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# create ament index resource which references the libraries in the binary dir
set(node_plugins "")

################################################################################
# manual composition example
 
add_executable(manual_composition
  src/manual_composition.cpp)

################################################################################
# Warn and be pedantic

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(manual_composition 
        PRIVATE "$<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic>")
    target_compile_options(manual_composition 
        PUBLIC "$<$<CONFIG:RELEASE>:-Wall -Wextra -Wpedantic>")
endif()

################################################################################
# add all compnent libraries 

target_link_libraries(manual_composition
  talker_component
  listener_component
  server_component
  client_component)

ament_target_dependencies(manual_composition
  "rclcpp"
  "geometry_msgs")
