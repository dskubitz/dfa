#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>

#include "ASTNode.h"

class Parser {
public:
    ASTNode* parse(std::string regexp);
private:
    bool match(char ch);
    bool check(char ch);
    char advance();
    bool at_end();
    char peek();
    char previous();
    char consume(char ch, const std::string& msg);

    ASTNode* expression();
    ASTNode* term();
    ASTNode* factor();
    ASTNode* primary();
    ASTNode* make_character_class(std::string&& str);
    ASTNode* make_negated_character_class(std::string&& str);

    std::string character_class();
    std::string range();

    std::string expr{};
    std::string::size_type pos{0};
    size_t leaf_count{0};
};

#endif //LEXER_PARSER_H
