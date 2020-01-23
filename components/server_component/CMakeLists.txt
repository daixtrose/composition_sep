cmake_minimum_required(VERSION 3.10)

project(server_component)

# Use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

add_library(${PROJECT_NAME} SHARED 
  src/server_component.cpp)

set_target_properties(${PROJECT_NAME} 
  PROPERTIES
  BUILD_RPATH_USE_ORIGIN ON
  INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib:/opt/ros/eloquent/lib"
  BUILD_RPATH "$ORIGIN:$ORIGIN/../lib:/opt/ros/eloquent/lib"
)
  
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

################################################################################
# Warn and be pedantic

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} 
        PRIVATE $<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic>)
    target_compile_options(${PROJECT_NAME} 
        PUBLIC $<$<CONFIG:RELEASE>:-Wall -Wextra -Wpedantic>)
endif()

################################################################################
# Find some external dependicies

find_package(ament_cmake REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)
find_package(std_msgs REQUIRED)

################################################################################
# Generate the export macro header for symbol visibility/Windows DLL declspec

include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME} 
    EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME}_export.h
    EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORT
    NO_EXPORT_MACRO_NAME ${PROJECT_NAME}_HIDDEN
    STATIC_DEFINE ITK_STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)  

# This indicates we want to have the "export" label when building the library     
target_compile_definitions(${PROJECT_NAME} PRIVATE "${PROJECT_NAME}_EXPORTS")

# Add the generated header to the install file list
install(FILES
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}_export.h
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
COMPONENT Development
)

################################################################################
# Ament needs some dependency hints

ament_target_dependencies(${PROJECT_NAME}
  "example_interfaces"
  "rclcpp"
  "rclcpp_components")


################################################################################
# Register the component

rclcpp_components_register_nodes(${PROJECT_NAME} "composition::Server")
set(node_plugins "${node_plugins}composition::Server;$<TARGET_FILE:server_component>\n")

################################################################################
# since the package installs libraries without exporting them
# it needs to make sure that the library path is being exported
if(NOT WIN32)
  ament_environment_hooks(
    "${ament_cmake_package_templates_ENVIRONMENT_HOOK_LIBRARY_PATH}")
endif()

install(TARGETS
  ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
