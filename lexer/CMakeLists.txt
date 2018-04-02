project(lexer)
add_library(lexer src/derivative.cpp src/dfa.cpp src/lexer.cpp src/parser.cpp src/regexp.cpp)
target_include_directories(lexer PUBLIC include)