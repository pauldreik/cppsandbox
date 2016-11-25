# Playing around with parallelizing std::partial_sum
# by Paul Dreik https://www.pauldreik.se/
# LICENSE: http://www.boost.org/LICENSE_1_0.txt
 
#this has only been tested on linux/gcc.
 
cmake_minimum_required(VERSION 2.8.7)
project(my_first_test)
enable_testing()
#SET(SANITIZEOPTS -fsanitize=address) #less then 2x slowdown
#SET(SANITIZEOPTS -fsanitize=thread) # approx 10x slowdown
#SET(SANITIZEOPTS -fsanitize=undefined)# approx 2x slowdown
#SET(PROFILEOPTS  -pg)
#SET(FASTMATHOPTS -ffast-math)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -g ${PROFILEOPTS} ${SANITIZEOPTS} ${FASTMATHOPTS}")

#file(GLOB SRC "*.h" "*.cpp")
set(SRC main.cpp unittest_unsignedint.cpp)

# creates the executable
add_executable(test_executable ${SRC})

#a benchmark
add_executable(benchmark benchmark.cpp)


# indicates the include paths
#target_include_directories(test_executable PRIVATE ${BOOST_INCLUDE_DIRS})

#boost_unit_test_framework

find_package(Boost 1.50.0 COMPONENTS unit_test_framework)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  #target_link_libraries(test_exectutable ${Boost_LIBRARIES})
  target_link_libraries(test_executable Boost::unit_test_framework)
endif()

target_link_libraries(test_executable pthread)
target_link_libraries(benchmark pthread)


# declares a test with our executable
add_test(NAME test1 COMMAND test_executable)
