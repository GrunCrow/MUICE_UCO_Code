CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(calibration)
ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -Wall")

FIND_PACKAGE(OpenCV REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(calibration calibration.cpp)
add_executable(video_example video_example.cpp)
add_executable(augReal augReal.cpp)
add_executable(augRealExtra augRealExtra.cpp)
 
