cmake_minimum_required(VERSION 4.3)
project(mini_redis)

set(CMAKE_CXX_STANDARD 20)

add_executable(mini_redis server.cpp
        client.cpp)
