#include <iostream>
#include <unordered_set>
#include <set>
#include <map>
#include "Lexer.h"
#include "Parser.h"

int main()
{
    Parser parser;
    std::vector<std::string> regexps = {
            "and",                          // 0
            "class",                        // 1
            "else",                         // 2
            "false",                        // 3
            "fun",                          // 4
            "for",                          // 5
            "if",                           // 6
            "nil",                          // 7
            "or",                           // 8
            "print",                        // 9
            "return",                       // 10
            "super",                        // 11
            "this",                         // 12
            "true",                         // 13
            "var",                          // 14
            "while",                        // 15
            "[A-Za-z_][A-Za-z_0-9]*",       // 16
            "[0-9]+(\\.[0-9]+)?",           // 17
            "\"(\\.|[^\\\"\n])*\"",         // 18
            "\\+",                          // 19
            "\\-",                          // 20
            "\\*",                          // 21
            "\\/",                          // 22
            "==",                           // 23
            "\\=",                          // 24
            "!=",                           // 25
            "\\!",                          // 26
            "<=",                           // 27
            "\\<",                          // 28
            ">=",                           // 29
            "\\>",                          // 30
            "\\(",                          // 31
            "\\)",                          // 32
            "\\{",                          // 33
            "\\}",                          // 34
            "\\[",                          // 35
            "\\]",                          // 36
            "\\;",                          // 37
            "[ \t\v\n\f]+",                 // 38
            ".",                            // 39
    };
    std::vector<Regexp> regular_vector = make_regular_vector(regexps);
    DFA dfa = make_DFA(regular_vector);

}