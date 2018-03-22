#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>

#include "Regex.h"

struct ParserError : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

class Parser {
public:
    std::unique_ptr<Regex> parse(const std::string& regexp)
    {
        return std::unique_ptr<Regex>(parse_impl(regexp));
    }

private:
    Regex* parse_impl(const std::string& regexp);

    bool match(char ch);
    bool check(char ch);
    char advance();
    bool at_end();
    char peek();
    char previous();
    char consume(char ch, const std::string& msg);

    Regex* expression();
    Regex* term();
    Regex* factor();
    Regex* primary();
    Regex* make_character_class(std::string&& str);
    Regex* make_negated_character_class(std::string&& str);

    std::string character_class();
    std::string range();

    std::string expr {};
    std::string::size_type pos {0};
    int paren_count {0};
};

#endif //LEXER_PARSER_H
