cmake_minimum_required(VERSION 2.24)


project( Processing )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Processing dataset_processing.cpp )
target_link_libraries( Processing ${OpenCV_LIBS} )