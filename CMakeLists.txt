cmake_minimum_required(VERSION 3.9)
project(lexer)
add_definitions(-std=c++1z -Wall -Wno-missing-braces -O3)

add_subdirectory(cmake)
add_subdirectory(test)

set(SOURCES Regexp.cpp Regexp.h
        Parser.cpp Parser.h
        DerivativeEvaluator.cpp DerivativeEvaluator.h
        NullableEvaluator.cpp NullableEvaluator.h
        DFA.cpp DFA.h
        DerivativeClassEvaluator.cpp DerivativeClassEvaluator.h
        Lexer.cpp Lexer.h
        )
add_library(core STATIC ${SOURCES})

target_include_directories(core PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR})

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(${Boost_INCLUDE_DIR})
# Note: a target should be already defined using 'add_executable' or 'add_library'
target_link_libraries(core ${Boost_LIBRARIES})

add_executable(main main.cpp Regexp.cpp Parser.cpp DerivativeEvaluator.cpp DerivativeClassEvaluator.cpp Lexer.cpp NullableEvaluator.cpp DFA.cpp)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR})

