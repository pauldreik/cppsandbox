cmake_minimum_required(VERSION 3.0)
project(tuplereorganizer)
file(GLOB_RECURSE sources *.cpp *.h)
add_executable(tuplereorganizer ${sources})
target_compile_options(tuplereorganizer PUBLIC -std=c++1y -Wall -Wextra -pedantic -Wfloat-conversion)

