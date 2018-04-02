project(rederiv)
cmake_minimum_required(VERSION 3.9)
add_definitions(-std=c++1z -Wall -Wno-missing-braces -Og)

add_subdirectory(cmake)
add_subdirectory(test)
add_subdirectory(lexer)
include_directories(lexer/include)
