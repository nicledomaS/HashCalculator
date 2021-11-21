cmake_minimum_required(VERSION 3.10)

project(hash_calculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

if(NOT make_initialize)
    include(cmake/make.cmake)
endif()

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
endif()

static_library(${PROJECT_NAME}
                SOURCES "src/"
                THREADS
                #BOOST system
                MODULES GSL)
