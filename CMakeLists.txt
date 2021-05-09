cmake_minimum_required(VERSION 3.1)
project(CV_Lab3)

set(CMAKE_GXX_FLAGS "-Wall -Wextra -Wconversion  -pedantic -std=gnu11")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab3 lab3.cpp)
target_link_libraries(lab3 ${OpenCV_LIBS})
