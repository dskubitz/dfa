cmake_minimum_required(VERSION 3.9)
project(lexer)

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)

add_executable(lexer main.cpp DFA.cpp DFA.h ASTNode.cpp ASTNode.h Printer.cpp Printer.h Parser.cpp Parser.h DFAFunctionCalculator.cpp DFAFunctionCalculator.h)