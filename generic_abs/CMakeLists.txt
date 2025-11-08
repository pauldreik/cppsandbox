cmake_minimum_required(VERSION 3.30)

project(generic_abs LANGUAGES CXX)

# ignore the build dir
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dependencies
find_package(fmt REQUIRED)

add_executable(generic_abs main.cpp)

target_link_libraries(generic_abs PRIVATE fmt::fmt)

# sanitizers
# target_compile_options(generic_abs PRIVATE -fsanitize=address,undefined)

# compiler warnings
target_compile_options(generic_abs PRIVATE -Wall -Wextra -pedantic)

include(GNUInstallDirs)
install(TARGETS generic_abs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
