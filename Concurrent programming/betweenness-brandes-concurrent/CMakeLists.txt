cmake_minimum_required(VERSION 3.1)
project(betweenness_brandes_concurrent)

find_package ( Threads REQUIRED )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -o2")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${CMAKE_THREAD_LIBS_INIT} )
    endif()
endmacro()

set(SOURCE_FILES brandes.cpp)
add_executable(betweenness_brandes_concurrent ${SOURCE_FILES})