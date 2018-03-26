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

RegexNode* Parser::parse_impl(const std::string& regexp)
{
    expr = regexp;
    pos = 0;
    paren_count = 0;
    try {
        if (expr.empty())
            return new Epsilon;
        return expression();
    }
    catch (std::out_of_range& e) {
        throw ParserError(e.what());
    }
    catch (ParserError&) {
        throw;
    }
}

RegexNode* Parser::expression()
{
    auto left = term();

    while (match('|')) {
        auto right = term();
        left = new Union(left, right);
    }

    return left;
}

RegexNode* Parser::term()
{
    auto left = factor();

    while (!at_end()) {
        char c = peek();

        if (c == '|') {
            break;
        } else if (c == ')') {
            if (paren_count == 0)
                error("unmatched parenthesis");
            --paren_count;
            break;
        } else {
            auto right = factor();
            left = new Concat(left, right);
        }
    }

    return left;
}

RegexNode* Parser::factor()
{
    auto left = primary();

    if (match('*'))
        left = new Closure(left);
    else if (match('+'))
        left = new Concat(left, new Closure(left->clone()));
    else if (match('?'))
        left = new Union(left, new Epsilon);

    return left;
}

RegexNode* Parser::primary()
{
    if (is_char(peek())) {
        return new Symbol(advance());
    } else if (match('\\')) {
        return new Symbol(advance());
    } else if (match('.')) {
        Bitset set;
        set.flip();
        return new Symbol(set);
    } else if (match('(')) {
        ++paren_count;
        auto expr = expression();
        consume(')', "unmatched parenthesis");
        return expr;
    } else if (match('[')) {
        Bitset set;
        if (match('^')) {
            set = character_class();
            set.flip();
        } else {
            set = character_class();
        }
        consume(']', "unmatched bracket");
        return new Symbol(set);
    } else {
        error("unrecognized");
    }
}

Bitset Parser::character_class()
{
    Bitset set = range();

    while (is_char(peek()))
        set |= range();

    return set;
}

Bitset Parser::range()
{
    Bitset set;

    char left = advance();
    set.set(static_cast<unsigned char>(left), true);

    if (!match('-'))
        return set;

    // If '-' is at the end of the character class it is not a range.
    if (peek() == ']') {
        set.set(static_cast<unsigned char>(left), true);
        return set;
    }

    char right = advance();

    if (left > right)
        error("start character greater than end character");

    while (left < right) {
        left++;
        set.set(static_cast<unsigned char>(left), true);
    }

    return set;
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
