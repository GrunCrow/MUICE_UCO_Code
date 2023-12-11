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
add_executable(1_stereo_calibrate 1_stereo_calibrate.cpp)
add_executable(2_stereo_checkundistorted 2_stereo_checkundistorted.cpp)
add_executable(3_stereo_disparity 3_stereo_disparity.cpp)
add_executable(4_stereo_sparse 4_stereo_sparse.cpp)
 
