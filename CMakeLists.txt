cmake_minimum_required(VERSION 3.5)

## cmake -DCMAKE_PREFIX_PATH=/opt/ros/eloquent ..

project(composition)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

################################################################################
# create ament index resource which references the libraries 
set(node_plugins "")

################################################################################
# Add libraries from subfolders

add_subdirectory(components/listener_component)
add_subdirectory(components/client_component)
add_subdirectory(components/server_component)
add_subdirectory(components/talker_component)

################################################################################
# search packages

find_package(ament_cmake REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)


################################################################################
# manual composition example
################################################################################
 
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
# add all component libraries 

# if(NOT APPLE)
#  set(CMAKE_INSTALL_RPATH $ORIGIN)
# endif() 

set_target_properties(manual_composition 
  PROPERTIES
  # CMAKE_BUILD_WITH_INSTALL_RPATH ON
  # CMAKE_INSTALL_RPATH_USE_LINK_PATH OFF
  CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib:/opt/ros/eloquent/lib"
  # CMAKE_BUILD_RPATH "$ORIGIN:$ORIGIN/../lib"
  )

target_link_libraries(manual_composition
  talker_component
  listener_component
  server_component
  client_component)

ament_target_dependencies(manual_composition
  "rclcpp"
  "geometry_msgs")


################################################################################
# linktime composition example
################################################################################

add_executable(linktime_composition
  src/linktime_composition.cpp)

################################################################################
# Warn and be pedantic

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
target_compile_options(linktime_composition 
    PRIVATE "$<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic>")
target_compile_options(linktime_composition 
    PUBLIC "$<$<CONFIG:RELEASE>:-Wall -Wextra -Wpedantic>")
endif()

################################################################################
# add all component libraries 

set(libs
  talker_component
  listener_component
  server_component
  client_component)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(libs
    "-Wl,--no-as-needed"
    ${libs}
    "-Wl,--as-needed")
endif()

target_link_libraries(linktime_composition ${libs})

ament_target_dependencies(linktime_composition
  "class_loader"
  "rclcpp"
  "rclcpp_components"
  "geometry_msgs")

################################################################################
# dlopen composition example
################################################################################

add_executable(dlopen_composition
  src/dlopen_composition.cpp)

ament_target_dependencies(dlopen_composition
  "class_loader"
  "rclcpp"
  "rclcpp_components")

install(TARGETS
  manual_composition
  linktime_composition
  dlopen_composition
  DESTINATION lib/${PROJECT_NAME})

ament_package()