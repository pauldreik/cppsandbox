cmake_minimum_required(VERSION 3.30)

project(MYPROJECTNAME LANGUAGES CXX)

# ignore the build dir
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dependencies
find_package(fmt REQUIRED)

add_executable(MYPROJECTNAME main.cpp)

target_link_libraries(MYPROJECTNAME PRIVATE fmt::fmt)

# sanitizers
# target_compile_options(MYPROJECTNAME PRIVATE -fsanitize=address,undefined)

# compiler warnings
target_compile_options(MYPROJECTNAME PRIVATE -Wall -Wextra -pedantic)

include(GNUInstallDirs)
install(TARGETS MYPROJECTNAME
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
