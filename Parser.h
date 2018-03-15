#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>

#include "ASTNode.h"

class Parser {
public:
    /**
     * The syntax trees produced by this parser are augmented by concatenation
     * of an endmarker.
     */
    ASTNode* parse(const std::string& regexp, const std::string& name);

    /**
     * Overload to allow for iterating over any range of pairs
     */
    ASTNode* parse(std::pair<std::string, std::string> pair)
    {
        return parse(pair.first, pair.second);
    }

    /**
     * Parse and unify any range of pair<string, string>
     */
    template<class Iter,
             class = typename std::enable_if<
                     std::is_convertible<
                             typename std::iterator_traits<Iter>::iterator_category,
                             std::input_iterator_tag>::value
                     && std::is_constructible<std::pair<std::string, std::string>,
                                              typename std::iterator_traits<Iter>::reference>::value>::type>
    ASTNode* parse(Iter b, Iter e)
    {
        if (b == e)
            throw std::runtime_error("create_regexes() with empty range");

        ASTNode* node = parse(*b++);
        for (; b != e; ++b) {
            node = unify(node, parse(*b));
        }
        return node;
    }

    ASTNode* parse(std::initializer_list<std::pair<std::string, std::string>> list)
    {
        return parse(list.begin(), list.end());
    }

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

    ASTNode* unify(ASTNode* left, ASTNode* right)
    {
        return new UnionNode(left, right);
    }

    std::string expr{};
    std::string::size_type pos{0};
};

#endif //LEXER_PARSER_H
