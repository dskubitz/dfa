#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>

#include "ASTNode.h"

struct ParserError : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

class Parser {
public:
    std::unique_ptr<ASTNode> parse(const std::string& regexp,
                                   const std::string& name = {})
    {
        return std::unique_ptr<ASTNode>(parse_impl(regexp, name));
    }

    /**
     * Overload to allow for iterating over any range of pairs
     */
    std::unique_ptr<ASTNode> parse(std::pair<std::string, std::string> pair)
    {
        return std::unique_ptr<ASTNode>(parse_impl(pair.first, pair.second));
    }

    template<class Iter,
             class = typename std::enable_if<
                     std::is_convertible<
                             typename std::iterator_traits<Iter>::iterator_category,
                             std::input_iterator_tag>::value
                     && std::is_constructible<std::pair<std::string,
                                                        std::string>,
                                              typename std::iterator_traits<Iter>::reference>::value>::type>
    std::unique_ptr<ASTNode> parse(Iter b, Iter e)
    {
        ASTNode* node = parse_impl(*b++);
        for (; b != e; ++b) {
            node = unify(node, parse_impl(*b));
        }
        return std::unique_ptr<ASTNode>(node);
    }

    std::unique_ptr<ASTNode> parse(
            std::initializer_list<std::pair<std::string, std::string>> list)
    {
        return parse(list.begin(), list.end());
    }

private:
    ASTNode* parse_impl(const std::string& regexp, const std::string& name);

    ASTNode* parse_impl(std::pair<std::string, std::string> pair)
    {
        return parse_impl(pair.first, pair.second);
    }

    ASTNode* unify(ASTNode* left, ASTNode* right)
    {
        return new UnionNode(left, right);
    }

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

    std::string expr {};
    std::string::size_type pos {0};
    int paren_count {0};
};

#endif //LEXER_PARSER_H
