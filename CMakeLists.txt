cmake_minimum_required(VERSION 3.9)
project(lexer)
add_definitions(-std=c++14)

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)
add_subdirectory(cmake)
add_subdirectory(test)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES TransitionTable.cpp ASTNode.cpp PrettyPrinter.cpp Parser.cpp TreeFunctions.cpp Lexer.cpp Lexer.h)
add_library(core STATIC ${SOURCES})
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(lexer main.cpp)
target_link_libraries(lexer core)
target_include_directories(lexer PRIVATE ${CMAKE_SOURCE_DIR})
