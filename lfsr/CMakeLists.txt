cmake_minimum_required(VERSION 3.25)

project(lfsr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

add_library(lfsr
    lfsr_coefficients.h
    lfsr.h
    lfsr_big.h
    lfsr_small.h
    bignum.cpp
    bignum.h
    integerselect.h
    lfsr_coefficients.h
)
target_include_directories(lfsr PUBLIC .)

add_executable(lfsrprog lfsrmain.cpp)
target_link_libraries(lfsrprog PRIVATE lfsr)

# tests
enable_testing()

add_executable(test_bignum test_bignum.cpp)
target_link_libraries(test_bignum PRIVATE lfsr Catch2::Catch2WithMain)
add_test(test_bignum test_bignum)

add_executable(test_integerselect test_integerselect.cpp)
target_link_libraries(test_integerselect PRIVATE lfsr Catch2::Catch2WithMain)
add_test(test_integerselect test_integerselect)

add_executable(test_lfsr_coefficients test_lfsr_coefficients.cpp)
target_link_libraries(test_lfsr_coefficients PRIVATE lfsr Catch2::Catch2WithMain)
add_test(test_lfsr_coefficients test_lfsr_coefficients)

add_executable(test_small_lfsr test_small_lfsr.cpp)
target_link_libraries(test_small_lfsr PRIVATE lfsr Catch2::Catch2WithMain)
add_test(test_small_lfsr test_small_lfsr)

add_executable(test_large_lfsr test_large_lfsr.cpp)
target_link_libraries(test_large_lfsr PRIVATE lfsr Catch2::Catch2WithMain)
add_test(test_large_lfsr test_large_lfsr)
