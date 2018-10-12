cmake_minimum_required(VERSION 3.1)

project(function)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include external/Catch2/single_include)

add_executable(test_function test/main.spec.cpp test/function.spec.cpp)
