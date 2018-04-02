#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>
#include <memory>

#include <lexer/regexp.h>

struct parser_error : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

class parser {
public:
    regexp parse(const std::string& expression)
    {
        return regexp(parse_impl(expression));
    }

private:
    Regex::Node* parse_impl(const std::string& regexp);

    bool match(char ch);
    bool check(char ch);
    char advance();
    bool at_end();
    char peek();
    char previous();
    char consume(char ch, const std::string& msg);

    Regex::Node* union_or_intersection();
    Regex::Node* concatenation();
    Regex::Node* prefix();
    Regex::Node* postfix();
    Regex::Node* factor();
    Regex::Node* character_class();

    Bitset parse_character_class();
    Bitset range();

    std::string expr {};
    std::string::size_type pos {0};
    int paren_count {0};
};

template<typename In, typename Out,
         typename = std::enable_if_t<
                 std::is_convertible_v<
                         typename std::iterator_traits<In>::iterator_category,
                         std::input_iterator_tag>>>
Out make_regular_vector(In beg, In end, Out it)
{
    static_assert(std::is_convertible_v<
            typename std::iterator_traits<Out>::iterator_category,
            std::output_iterator_tag>, "Invalid output iterator");

    parser parser;

    for (; beg != end; ++beg)
        *it++ = parser.parse(*beg);

    return it;
}

template<template<typename, typename...> class Cont, typename T, typename ...A>
std::vector<regexp> make_regular_vector(const Cont<T, A...>& cont)
{
    std::vector<regexp> res;
    res.reserve(std::size(cont));
    make_regular_vector(std::begin(cont), std::end(cont),
                        std::back_inserter(res));

    return res;
}

template<typename T>
std::vector<regexp> make_regular_vector(std::initializer_list<T> list)
{
    std::vector<regexp> res;
    res.reserve(list.size());
    make_regular_vector(list.begin(), list.end(), std::back_inserter(res));

    return res;
}

#endif //LEXER_PARSER_H
