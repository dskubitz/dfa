project(lexer)
add_library(lexer src/Derivative.cpp src/DFA.cpp src/Scanner.cpp src/Parser.cpp src/Regexp.cpp src/Compiler.cpp include/lexer/Compiler.h)
target_include_directories(lexer PUBLIC include)
find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(lexer ${Boost_LIBRARIES})
