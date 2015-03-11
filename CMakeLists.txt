cmake_minimum_required(VERSION 2.8.12)
project(punchr)

#YCM support
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

#boost
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(include src ${Boost_INCLUDE_DIRS})
add_executable(punchr src/punchr.cpp)
target_link_libraries(punchr ${Boost_LIBRARIES})
