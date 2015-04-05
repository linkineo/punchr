cmake_minimum_required(VERSION 2.8.12)
project(punchr)

#YCM support
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

#cpp 11
set(CMAKE_CXX_FLAGS "-std=c++0x")

#boost
find_package(Boost COMPONENTS program_options date_time  REQUIRED)

include_directories(include src ${Boost_INCLUDE_DIRS})
add_executable(punchr src/punchr.cpp src/timesheet.cpp src/timeSheetPersistency.cpp)
target_link_libraries(punchr ${Boost_LIBRARIES} jsoncpp)
