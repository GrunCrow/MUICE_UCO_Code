CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(edge_detector)
ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -Wall")

FIND_PACKAGE(OpenCV REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(edge_detector edge_detector.cpp common_code.hpp common_code.cpp)
add_executable(edge_detector_test_common_code test_common_code.cpp common_code.cpp common_code.hpp)
set_target_properties(edge_detector_test_common_code PROPERTIES OUTPUT_NAME "test_common_code")

