cmake_minimum_required (VERSION 3.15)
project (dedup)
set(CMAKE_CXX_FLAGS "-Wall -pipe -O2 -g --std=c++17")
set(SOURCES main.cc)

add_executable(dedup ${SOURCES})

