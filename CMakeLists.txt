cmake_minimum_required(VERSION 3.29)
project(systemy-operacyjne)

set(CMAKE_CXX_STANDARD 20)

add_executable(systemy-operacyjne main.cpp
        philosopher.h
        philosopher.cpp)
