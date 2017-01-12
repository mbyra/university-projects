cmake_minimum_required(VERSION 3.6)
project(horror_movie_2)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES helper.cc helper.h monster.cc monster.h citizen.cc citizen.h smalltown.cc smalltown.h horror_example.cc)
add_executable(horror_movie_2 ${SOURCE_FILES})