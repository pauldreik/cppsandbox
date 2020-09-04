cmake_minimum_required(VERSION 3.11)

project(floatingpoint LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework)

add_executable(floatingpoint main.cpp FloatingPoint.hpp)

enable_testing()
add_executable(testfloat test.cpp FloatingPoint.hpp)
target_link_libraries(testfloat Boost::unit_test_framework)
