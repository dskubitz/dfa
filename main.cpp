#include "DFA.h"
#include "Regexp.h"
#include "Parser.h"
#include "Lexer.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

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

    //@formatter:off
    std::string str {
             "class Tree {\n"
             "  init(item, depth) {\n"
             "    this.item = item;\n"
             "    this.depth = depth;\n"
             "    if (depth > 0) {\n"
             "      var item2 = item + item;\n"
             "      depth = depth - 1;\n"
             "      this.left = Tree(item2 - 1, depth);\n"
             "      this.right = Tree(item2, depth);\n"
             "    } else {\n"
             "      this.left = nil;\n"
             "      this.right = nil;\n"
             "    }\n"
             "  }\n"
             "\n"
             "  check() {\n"
             "    if (this.left == nil) {\n"
             "      return this.item;\n"
             "    }\n"
             "\n"
             "    return this.item + this.left.check() - this.right.check();\n"
             "  }\n"
             "}\n"
             "\n"
             "var minDepth = 4;\n"
             "var maxDepth = 12;\n"
             "var stretchDepth = maxDepth + 1;\n"
             "\n"
             "var start = clock();\n"
             "\n"
             "print \"stretch tree of depth:\";\n"
             "print stretchDepth;\n"
             "print \"check:\";\n"
             "print Tree(0, stretchDepth).check();\n"
             "\n"
             "var longLivedTree = Tree(0, maxDepth);\n"
             "\n"
             "// iterations = 2 ** maxDepth\n"
             "var iterations = 1;\n"
             "var d = 0;\n"
             "while (d < maxDepth) {\n"
             "  iterations = iterations * 2;\n"
             "  d = d + 1;\n"
             "}\n"
             "\n"
             "var depth = minDepth;\n"
             "while (depth < stretchDepth) {\n"
             "  var check = 0;\n"
             "  var i = 1;\n"
             "  while (i <= iterations) {\n"
             "    check = check + Tree(i, depth).check() + Tree(-i, depth).check();\n"
             "    i = i + 1;\n"
             "  }\n"
             "\n"
             "  print \"num trees:\";\n"
             "  print iterations * 2;\n"
             "  print \"depth:\";\n"
             "  print depth;\n"
             "  print \"check:\";\n"
             "  print check;\n"
             "\n"
             "  iterations = iterations / 4;\n"
             "  depth = depth + 2;\n"
             "}\n"
             "\n"
             "print \"long lived tree of depth:\";\n"
             "print maxDepth;\n"
             "print \"check:\";\n"
             "print longLivedTree.check();\n"
             "print \"elapsed:\";\n"
             "print clock() - start;"
    };
    //@formatter:on
    std::stringstream iss(str);

    Lexer lexer(std::move(dfa), iss);

    auto error = regexps.size() - 1;
    auto space = regexps.size() - 2;

    while (!lexer.end_of_file()) {
        auto tok = static_cast<unsigned>(lexer.scan());
        if (tok == error || tok == space) continue;
        std::cout << lexer.lexeme() << " tok: " << tok
                  << ' ' << lexer.lexeme_start_position()
                  << ' ' << lexer.current_position() << '\n';
    }
}