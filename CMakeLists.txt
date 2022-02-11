cmake_minimum_required (VERSION 3.15)
project (dedup)
set(CMAKE_CXX_FLAGS "-Wall -pipe -O2 -march=native -g --std=c++17")
set(SOURCES main.cc)

add_executable(dedup ${SOURCES})
target_include_directories(dedup PUBLIC /usr/local/opt/openssl/include)
target_link_directories(dedup PUBLIC /usr/local/opt/openssl/lib)
target_link_libraries(dedup ssl crypto)
