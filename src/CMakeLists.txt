cmake_minimum_required(VERSION 3.10)
project(levelnet)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

set(
        _LEVEL_CONFIG
        config/config.h config/config.cc
)
set(
        _LEVEL_NET_
        net/epoll.h net/epoll.cc
        net/buffer.h net/buffer.cc
        net/channel.h net/channel.cc
        net/connection.h net/connection.cc
        net/event_loop.h net/event_loop.cc
        net/conn_pool.h net/conn_pool.cc
        net/timer_queue.h net/timer_queue.cc
)
set(
        _LEVEL_EXECUTE
        ${_LEVEL_NET_}
        ${_LEVEL_CONFIG}
)
add_subdirectory(utility/logger)
add_executable(levelnet main.cc ${_LEVEL_EXECUTE})