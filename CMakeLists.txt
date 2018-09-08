cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(BIGINT
        #main.cpp
        big_integer.h
        big_integer.cpp
        big_integer_testing.cpp

        vector/vector.h
        vector/vector.cpp
        vector/CoW.h
        vector/CoW.cpp

        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-parentheses -pedantic")

