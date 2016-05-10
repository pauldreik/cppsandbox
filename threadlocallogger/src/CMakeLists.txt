file(GLOB srces
    "*.h"
    "*.cpp"
)
add_executable (main  ${srces})
#target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(main pthread)
