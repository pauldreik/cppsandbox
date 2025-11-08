cmake_minimum_required(VERSION 3.30)

project(nomultiplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ignore the build dir
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

# dependencies
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)

add_library(nomultiplication)
target_sources(nomultiplication
    PUBLIC
    multiplication.h
    PRIVATE
    multiplication.cpp
)
target_include_directories(nomultiplication PUBLIC .)

add_executable(benchmark_division benchmark_division.cpp)
target_link_libraries(benchmark_division PRIVATE nomultiplication Catch2::Catch2WithMain)

add_executable(benchmark_equality benchmark_equality.cpp)
target_link_libraries(benchmark_equality PRIVATE nomultiplication Catch2::Catch2WithMain)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE nomultiplication Catch2::Catch2WithMain)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_executable(fuzzer fuzzer.cpp)
    target_link_libraries(fuzzer PRIVATE nomultiplication)
    target_include_directories(fuzzer PRIVATE .)
    target_compile_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined)
    target_link_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined)
endif  ()


