CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(cbg_process)
ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -Wall")

# Encuentra y configura Qt
#find_package(Qt5 COMPONENTS Core REQUIRED)
#include_directories(${Qt5Core_INCLUDE_DIRS})

FIND_PACKAGE(OpenCV REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(cbg_process cbg_process.cpp common_code.cpp
    common_code.hpp)

# Enlaza las bibliotecas necesarias (Qt y OpenCV)
#target_link_libraries(cbg_process PRIVATE Qt5::Core ${OpenCV_LIBS})

add_executable(cbg_process_test_common_code test_common_code.cpp common_code.cpp
    common_code.hpp)

set_target_properties(cbg_process_test_common_code PROPERTIES OUTPUT_NAME "test_common_code")
