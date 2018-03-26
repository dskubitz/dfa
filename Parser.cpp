#include "Parser.h"

namespace {
bool is_meta(char ch)
{
    return ch == '*' || ch == '+' || ch == '-' || ch == '|' || ch == '.'
           || ch == '(' || ch == ')' || ch == '[' || ch == ']'
           || ch == '?' || ch == '\\' || ch == '&';
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
        return union_or_intersection();
    }
    catch (std::out_of_range& e) {
        throw ParserError(e.what());
    }
    catch (ParserError&) {
        throw;
    }
}

RegexNode* Parser::union_or_intersection()
{
    auto left = concatenation();

    while (true) {
        if (match('|')) {
            auto right = concatenation();
            left = new Union(left, right);
            continue;
        } else if (match('&')) {
            auto right = concatenation();
            left = new Intersection(left, right);
            continue;
        }
        break;
    }

    return left;
}

RegexNode* Parser::concatenation()
{
    auto left = postfix();

    while (!at_end()) {
        char c = peek();

        if (c == '|' || c == '&') {
            break;
        } else if (c == ')') {
            if (paren_count == 0)
                error("unmatched parenthesis");
            --paren_count;
            break;
        } else {
            auto right = postfix();
            left = new Concat(left, right);
        }
    }

    return left;
}

RegexNode* Parser::postfix()
{
    auto left = factor();

    if (match('*'))
        left = new Closure(left);
    else if (match('+'))
        left = new Concat(left, new Closure(left->clone()));
    else if (match('?'))
        left = new Union(left, new Epsilon);

    return left;
}

RegexNode* Parser::factor()
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
        auto expr = union_or_intersection();
        consume(')', "unmatched parenthesis");
        return expr;
    } else if (match('[')) {
        return character_class();
    } else {
        error("unrecognized");
    }
}

RegexNode* Parser::character_class()
{
    Bitset set;
    if (match('^')) {
        set = parse_character_class();
        set.flip();
    } else if (match('-')) {
        set = parse_character_class();
        set.set('-', true);
    } else {
        set = parse_character_class();
    }

    if (peek() == '-') {
        advance();
        set.set('-', true);
    }

    consume(']', "unmatched bracket");
    return new Symbol(set);
}

Bitset Parser::parse_character_class()
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
