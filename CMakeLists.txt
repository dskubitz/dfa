cmake_minimum_required(VERSION 3.9)
project(lexer)
add_definitions(-std=c++1z)

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)
add_subdirectory(cmake)
add_subdirectory(test)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES Regex.cpp Parser.cpp Derivative.cpp Derivative.h Nullable.cpp Nullable.h TransitionTable.cpp)
add_library(core STATIC ${SOURCES})
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR})
