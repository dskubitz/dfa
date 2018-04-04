#include <lexer/Regexp.h>
#include "gtest/gtest.h"
#include "lexer/Compiler.h"

TEST(CompilerTests, TestTests)
{
    std::string str(
            "NL          \\n\n"
                    "WS          [ \\t]+\n"
                    "AND         and\n"
                    "IDENTIFIER  [A-Za-z_][A-Za-z_0-9]*\n"
                    "CONSTANT    0[0-7]*\n"
                    "CONSTANT    0x[A-Fa-f0-9]*\n"
    );
    std::stringstream input(str);
    std::stringstream output;
    Compiler compiler;
    compiler.compile(input, output);
    for (std::string line; std::getline(output, line);) {
        std::cout << line << '\n';
    }
}
