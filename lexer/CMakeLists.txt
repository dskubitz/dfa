project(lexer)
add_library(lexer src/Derivative.cpp src/DFA.cpp src/Scanner.cpp src/Parser.cpp src/Regexp.cpp)
target_include_directories(lexer PUBLIC include)