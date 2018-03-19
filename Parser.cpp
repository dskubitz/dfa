#include <vector>
#include <sstream>
#include <iostream>

#include "Parser.h"

namespace {
bool is_meta(char ch)
{
    return ch == '*' || ch == '+' || ch == '-' || ch == '|' || ch == '.'
           || ch == '(' || ch == ')' || ch == '[' || ch == ']'
           || ch == '?' || ch == '\\';
}

bool is_char(char ch)
{
    return !is_meta(ch);
}

[[noreturn]] void error(const std::string& msg)
{
    throw ParserError{msg};
}
}

ASTNode* Parser::parse_impl(const std::string& regexp, const std::string& name)
{
    expr = regexp;
    pos = 0;
    paren_count = 0;
    try {
        return new CatNode(expression(), new EndmarkerNode(name));
    }
    catch (std::out_of_range& e) {
        throw ParserError(e.what());
    }
    catch (ParserError&) {
        throw;
    }
}

ASTNode* Parser::expression()
{
    auto left = term();

    while (match('|')) {
        auto right = term();
        left = new UnionNode(left, right);
    }

    return left;
}

ASTNode* Parser::term()
{
    auto left = factor();

    while (!at_end()) {
        char c = peek();

        if (c == '|') {
            break;
        } else if (c == ')') {
            if (paren_count==0)
                error("unmatched parenthesis");
            --paren_count;
            break;
        } else {
            auto right = factor();
            left = new CatNode(left, right);
        }
    }

    return left;
}

ASTNode* Parser::factor()
{
    auto left = primary();

    if (match('*')) {
        left = new StarNode(left);
    } else if (match('+')) {
        left = new CatNode(left, new StarNode(left->clone()));
    } else if (match('?')) {
        left = new UnionNode(left, new EpsilonNode);
    }

    return left;
}

ASTNode* Parser::primary()
{
    if (is_char(peek())) {
        return new CharNode(advance());
    } else if (match('\\')) {
        return new CharNode(advance());
    } else if (match('.')) {
        ASTNode* left = new CharNode(9);
        auto it = alphabet.begin() + 1, end = alphabet.end();
        for (; it != end; ++it) {
            char ch = *it;
            if (ch == '\n' || ch == '\r')
                continue;
            auto right = new CharNode(ch);
            left = new UnionNode(left, right);
        }
        return left;
    } else if (match('(')) {
        ++paren_count;
        auto expr = expression();
        consume(')', "unmatched parenthesis");
        return expr;
    } else if (match('[')) {
        ASTNode* class_expr = nullptr;
        if (match('^'))
            class_expr = make_negated_character_class(character_class());
        else
            class_expr = make_character_class(character_class());
        consume(']', "unmatched bracket");
        return class_expr;
    } else {
        error("unrecognized");
    }
}

ASTNode* Parser::make_character_class(std::string&& str)
{
    auto it = str.begin();
    ASTNode* expr = new CharNode(*it++);
    while (it != str.end()) {
        char ch = *it++;
        expr = new UnionNode(expr, new CharNode(ch));
    }
    return expr;
}

ASTNode* Parser::make_negated_character_class(std::string&& str)
{
    std::vector<char> chars(128);

    for (auto c : str) {
        chars[static_cast<int>(c)] = 1;
    }

    ASTNode* expr = nullptr;

    // Can't meaningfully match ascii nul, so start at ws chars
    size_t it = 9;
    size_t end = chars.size();

    // Find the first unset value in chars
    while (it != end)
        if (chars[it] == 0) {
            expr = new CharNode(static_cast<char>(it++));
            break;
        }

    for (; it != end; ++it)
        if (chars[it] == 0 && (isgraph(it) || isspace(it)))
            expr = new UnionNode(expr, new CharNode(static_cast<char>(it)));

    return expr;
}

std::string Parser::character_class()
{
    std::string res;
    res += range();

    while (is_char(peek())) {
        auto right = range();
        res += right;
    }

    return res;
}

std::string Parser::range()
{
    std::stringstream ss;

    auto left = advance();
    ss << left;

    if (!match('-'))
        return ss.str();

    // If '-' is at the end of the character class it is not a range.
    if (peek() == ']') {
        ss << previous();
        return ss.str();
    }

    auto right = advance();

    if (left > right)
        error("start character greater than end character");

    while (left < right) {
        left++;
        ss << left;
    }
    return ss.str();
}

bool Parser::match(char ch)
{
    if (check(ch)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(char ch)
{
    return !at_end() && peek() == ch;
}

char Parser::advance()
{
    ++pos;
    return previous();
}

bool Parser::at_end()
{
    return pos == expr.length();
}

char Parser::peek()
{
    return expr.at(pos);
}

char Parser::previous()
{
    return expr.at(pos - 1);
}

char Parser::consume(char ch, const std::string& msg)
{
    if (check(ch))
        return advance();
    error(msg);
}


