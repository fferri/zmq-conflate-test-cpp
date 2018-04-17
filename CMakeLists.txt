cmake_minimum_required(VERSION 3.1)

project(zmq-conflate-test-cpp)

include(CheckIncludeFileCXX)
include(CheckCSourceRuns)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR 1)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    # clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # gcc
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # visual c++
endif()
set(CMAKE_AUTOMOC ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    add_definitions(-DBOOST_ALL_NO_LIB)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DBOOST_USE_WINDOWS_H)
    add_definitions(-DNOMINMAX)
endif()

find_package(Boost 1.54 REQUIRED COMPONENTS thread system regex timer filesystem serialization)
find_package(ZMQ 4.1.4 REQUIRED)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cppzmq/zmq.hpp)
    execute_process(COMMAND git submodule init WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND git submodule update WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cppzmq)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${ZMQ_INCLUDE_DIRS})

add_executable(proxy proxy.cpp)
target_link_libraries(proxy ${ZMQ_LIBRARY} ${Boost_LIBRARIES})

add_executable(pub pub.cpp)
target_link_libraries(pub ${ZMQ_LIBRARY} ${Boost_LIBRARIES})

add_executable(sub sub.cpp)
target_link_libraries(sub ${ZMQ_LIBRARY} ${Boost_LIBRARIES})

if(WIN32)
    target_link_libraries(proxy wsock32 ws2_32)
    target_link_libraries(pub wsock32 ws2_32)
    target_link_libraries(sub wsock32 ws2_32)
endif()

