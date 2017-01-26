cmake_minimum_required(VERSION 3.6)
project(graph2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES circuit.c graph.c graph.h stack.c stack.h parser.c parser.h err.c err.h)
add_executable(graph2 ${SOURCE_FILES})