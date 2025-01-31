cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    logic_gates
    VERSION 0.1.0
    DESCRIPTION "testing generation of  logic gates"
    HOMEPAGE_URL "https://www.pauldreik.se/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    logic_gates_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    logic_gates_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(logic_gates_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(logic_gates_exe source/main.cpp)
add_executable(logic_gates::exe ALIAS logic_gates_exe)

set_property(TARGET logic_gates_exe PROPERTY OUTPUT_NAME logic_gates)

target_compile_features(logic_gates_exe PRIVATE cxx_std_20)

target_link_libraries(logic_gates_exe PRIVATE logic_gates_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT logic_gates_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of logic_gates"
  )
endif()

include(cmake/dev-mode.cmake)
