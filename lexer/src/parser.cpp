#include <lexer/parser.h>

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
    throw parser_error{msg};
}

Regex::Node* parser::parse_impl(const std::string& regexp)
{
    expr = regexp;
    pos = 0;
    paren_count = 0;
    try {
        if (expr.empty())
            return new Regex::Epsilon;
        else if (expr == ".") {
            Bitset set;
            set.flip();
            return new Regex::Symbol(set);
        }
        return union_or_intersection();
    }
    catch (std::out_of_range& e) {
        throw parser_error(e.what());
    }
    catch (parser_error&) {
        throw;
    }
}

Regex::Node* parser::union_or_intersection()
{
    std::unique_ptr<Regex::Node> left(concatenation());

    while (true) {
        if (match('|')) {
            std::unique_ptr<Regex::Node> right(concatenation());
            left.reset(make_union(left.release(), right.release()));
            continue;
        } else if (match('&')) {
            std::unique_ptr<Regex::Node> right(concatenation());
            left.reset(make_intersection(left.release(), right.release()));
            continue;
        }
        break;
    }

    return left.release();
}

Regex::Node* parser::concatenation()
{
    std::unique_ptr<Regex::Node> left(prefix());

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
            std::unique_ptr<Regex::Node> right(prefix());
            left.reset(make_concatenation(left.release(), right.release()));
        }
    }

    return left.release();
}

Regex::Node* parser::prefix()
{
    if (match('~')) {
        std::unique_ptr<Regex::Node> right(prefix());
        return make_complement(right.release());
    }
    return postfix();
}

Regex::Node* parser::postfix()
{
    std::unique_ptr<Regex::Node> left(factor());

    if (match('*'))
        left.reset(make_closure(left.release()));
    else if (match('+')) {
        auto p = left.release();
        left.reset(make_concatenation(p, make_closure(p->clone())));
    } else if (match('?'))
        left.reset(make_union(left.release(), new Regex::Epsilon));

    return left.release();
}

Regex::Node* parser::factor()
{
    if (is_char(peek())) {
        return new Regex::Symbol(advance());
    } else if (match('\\')) {
        return new Regex::Symbol(advance());
    } else if (match('.')) {
        Bitset set;
        set.flip();
        return new Regex::Symbol(set);
    } else if (match('(')) {
        ++paren_count;
        std::unique_ptr<Regex::Node> expr(union_or_intersection());
        consume(')', "unmatched parenthesis");
        return expr.release();
    } else if (match('[')) {
        return character_class();
    } else {
        error("unrecognized");
    }
}

Regex::Node* parser::character_class()
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
    return new Regex::Symbol(set);
}

Bitset parser::parse_character_class()
{
    Bitset set = range();

    while (is_char(peek()))
        set |= range();

    return set;
}

Bitset parser::range()
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

bool parser::match(char ch)
{
    if (check(ch)) {
        advance();
        return true;
    }
    return false;
}

bool parser::check(char ch)
{
    return !at_end() && peek() == ch;
}

char parser::advance()
{
    ++pos;
    return previous();
}

bool parser::at_end()
{
    return pos == expr.length();
}

char parser::peek()
{
    return expr.at(pos);
}

char parser::previous()
{
    return expr.at(pos - 1);
}

char parser::consume(char ch, const std::string& msg)
{
    if (check(ch))
        return advance();
    error(msg);
}

