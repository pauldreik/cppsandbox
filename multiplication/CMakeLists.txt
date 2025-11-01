cmake_minimum_required(VERSION 3.30)

project(nomultiplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ignore the build dir
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

# dependencies
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)

add_executable(nomultiplication main.cpp)
target_link_libraries(nomultiplication PRIVATE fmt::fmt)
target_include_directories(nomultiplication PRIVATE .)

add_executable(benchmark_division benchmark_division.cpp)
target_link_libraries(benchmark_division Catch2::Catch2WithMain)

add_executable(benchmark_equality benchmark_equality.cpp)
target_link_libraries(benchmark_equality Catch2::Catch2WithMain)
target_include_directories(benchmark_equality PRIVATE .)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_executable(fuzzer fuzzer.cpp)
    target_include_directories(fuzzer PRIVATE .)
    target_compile_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined)
    target_link_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined)
endif  ()

# sanitizers
# target_compile_options(MYPROJECTNAME PRIVATE -fsanitize=address,undefined)

# compiler warnings
target_compile_options(nomultiplication PRIVATE -Wall -Wextra -pedantic)

include(GNUInstallDirs)
install(TARGETS nomultiplication
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
