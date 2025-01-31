install(
    TARGETS logic_gates_exe
    RUNTIME COMPONENT logic_gates_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
