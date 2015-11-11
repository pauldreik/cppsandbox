cmake_minimum_required(VERSION 3.0)
project(compiletimelog)
file(GLOB_RECURSE sources *.cpp *.h)
add_executable(compiletimelog ${sources})
target_compile_options(compiletimelog PUBLIC -std=c++1y -Wall -Wextra -pedantic -Wfloat-conversion)

