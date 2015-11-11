cmake_minimum_required(VERSION 3.0)
project(fizzbuzz)
file(GLOB_RECURSE sources *.cpp *.h)
add_executable(fizzbuzz ${sources})
target_compile_options(fizzbuzz PUBLIC -std=c++1y -Wall -Wextra -pedantic -Wfloat-conversion)

