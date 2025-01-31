# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(logic_gatesTests LANGUAGES CXX)

# ---- Tests ----

add_executable(logic_gates_test source/logic_gates_test.cpp)
target_link_libraries(logic_gates_test PRIVATE logic_gates_lib)
target_compile_features(logic_gates_test PRIVATE cxx_std_20)

add_test(NAME logic_gates_test COMMAND logic_gates_test)

# ---- End-of-file commands ----

add_folders(Test)
