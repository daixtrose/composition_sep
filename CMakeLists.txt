cmake_minimum_required(VERSION 3.5)

## cmake -DCMAKE_PREFIX_PATH=/opt/ros/eloquent ..

project(composition)

set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

# Probably better off with 
# https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html ?
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

################################################################################
# Import external dependencies

include(FetchContent)

FetchContent_Declare(
  rxcpp
  GIT_REPOSITORY https://github.com/ReactiveX/RxCpp
  GIT_TAG master
  # SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps     
)

FetchContent_GetProperties(rxcpp)

if(NOT rxcpp_POPULATED)
  FetchContent_Populate(rxcpp)
  add_subdirectory(${rxcpp_SOURCE_DIR} ${rxcpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


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
# Add sanitizer utilities 
include(CMakeSanitizers.cmake)

################################################################################
# manual composition example
################################################################################
 
add_executable(manual_composition
  src/manual_composition.cpp)

################################################################################
# Warn and be pedantic

# target_compile_options(rxcpp
#   INTERFACE
#   -Wno-error=deprecated)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(manual_composition 
        PRIVATE $<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic -Werror -Wno-error=deprecated>)
    target_compile_options(manual_composition 
        PUBLIC $<$<CONFIG:RELEASE>:-Wall -Wextra -Wpedantic -Werror -Wno-error=deprecated>)
endif()

target_sanitize(manual_composition)

################################################################################
# add all component libraries 

# if(NOT APPLE)
#  set(CMAKE_INSTALL_RPATH $ORIGIN)
# endif() 

set_target_properties(manual_composition 
  PROPERTIES
  # CMAKE_BUILD_WITH_INSTALL_RPATH ON
  # CMAKE_INSTALL_RPATH_USE_LINK_PATH OFF
  BUILD_RPATH_USE_ORIGIN ON
  INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib:/opt/ros/eloquent/lib"
  BUILD_RPATH "$ORIGIN:$ORIGIN/../lib:/opt/ros/eloquent/lib"
  )

target_include_directories(manual_composition 
  BEFORE PUBLIC ${rxcpp_SOURCE_DIR}/Rx/v2/src)

target_link_libraries(manual_composition PUBLIC
  rxcpp
  talker_component
  listener_component
  server_component
  client_component)

ament_target_dependencies(manual_composition PUBLIC
  "rclcpp"
  "geometry_msgs"
  "std_msgs")

################################################################################
# linktime composition example
################################################################################

add_executable(linktime_composition
  src/linktime_composition.cpp)

################################################################################
# Warn and be pedantic

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
target_compile_options(linktime_composition 
    PRIVATE $<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic>)
target_compile_options(linktime_composition 
    PUBLIC $<$<CONFIG:RELEASE>:-Wall -Wextra -Wpedantic>)
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

target_link_libraries(linktime_composition PUBLIC ${libs})

ament_target_dependencies(linktime_composition 
  PUBLIC 
  "class_loader"
  "rclcpp"
  "rclcpp_components"
  "geometry_msgs")

################################################################################
# dlopen composition example
################################################################################

add_executable(dlopen_composition
  src/dlopen_composition.cpp)

ament_target_dependencies(dlopen_composition PUBLIC
  "class_loader"
  "rclcpp"
  "rclcpp_components")

################################################################################
# Packaging configuration
# Debian installer prefix propagated to CMakeCPackConfig
set(DEB_INSTALL_PREFIX "/opt/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
configure_file(CMakeCPackConfig.cmake.in CMakeCPackConfig.cmake @ONLY)
include(${CMAKE_SOURCE_DIR}/CMakeCPack.cmake)

install(TARGETS
  manual_composition
  linktime_composition
  dlopen_composition
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(PROGRAMS src/start_manual_composition.sh DESTINATION bin)


ament_package()