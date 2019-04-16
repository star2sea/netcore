cmake_minimum_required(VERSION 3.0)
project(netcore)


set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(UNIX)
	add_definitions("-lpthread")
ENDIF(UNIX)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(thirdparty/install/include)
link_directories(${PROJECT_BINARY_DIR}/libs)
link_directories(thirdparty/install/lib)

macro(add_vg_source_dir name)
    file(GLOB SRC_FILE ${name}/*.h ${name}/*.hpp ${name}/*.cc ${name}/*.cpp)
    list(APPEND ALL_SRC ${SRC_FILE})
    source_group(${name} FILES ${SRC_FILE})
endmacro()

add_vg_source_dir(net)
add_vg_source_dir(utils)

add_library(netcore STATIC ${ALL_SRC})

add_subdirectory(test)
