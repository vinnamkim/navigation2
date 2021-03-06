cmake_minimum_required(VERSION 3.5)
project(dwb_controller)

find_package(ament_cmake REQUIRED)
find_package(nav2_common REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav2_util REQUIRED)
find_package(nav2_robot REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(dwb_core REQUIRED)
find_package(nav_2d_utils REQUIRED)
find_package(nav_2d_msgs REQUIRED)

nav2_package()

include_directories(
  include
)

set(executable_name dwb_controller)

add_executable(${executable_name}
  src/main.cpp
)

set(library_name ${executable_name}_core)

add_library(${library_name}
  src/dwb_controller.cpp
  src/progress_checker.cpp
)

set(dependencies
  rclcpp
  rclcpp_action
  std_msgs
  nav2_robot
  nav2_msgs
  dwb_core
  nav_2d_utils
  nav_2d_msgs
  nav2_util
)

ament_target_dependencies(${library_name}
  ${dependencies}
)

ament_target_dependencies(${executable_name}
  ${dependencies}
)

target_link_libraries(${executable_name} ${library_name})

install(TARGETS ${executable_name} ${library_name}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)

ament_package()
