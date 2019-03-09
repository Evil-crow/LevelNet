cmake_minimum_required(VERSION 3.10)
project(logger)

set(CMAKE_CXX_STANDARD 14)

add_library(logger logger.cc logger.h
        log_file.h log_file.cc
        async_logging.h async_logging.cc
        log_stream.h log_stream.cc
        log_buffer.h log_buffer.cc)