#ifndef LEXER_H
#define LEXER_H

#include <tuple>
#include <istream>
#include <stdexcept>
#include <unordered_map>

class TransitionTable;

struct LexerError : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

struct Token {
    std::string lexeme;
    int kind;
    int line;
    int column;

    friend bool operator==(const Token& lhs, const Token& rhs)
    {
        return std::tie(lhs.lexeme, lhs.kind, lhs.line, lhs.column)
               == std::tie(rhs.lexeme, rhs.kind, rhs.line, rhs.column);
    }

    friend bool operator!=(const Token& lhs, const Token& rhs)
    {
        return !(rhs == lhs);
    }
};

class Lexer {
public:
    Lexer(const TransitionTable& t, std::istream& i);

    Token scan();

    const std::unordered_map<std::string, int>&
    stoitoken_kinds() const noexcept { return stoitoken_kinds_; }

    const std::unordered_map<int, std::string>&
    itostoken_kinds() const noexcept { return itostoken_kinds_; }

    const TransitionTable& table() const noexcept { return table_; }

    bool good() { return input_.good(); }

private:
    const TransitionTable& table_;
    std::istream& input_;
    std::unordered_map<std::string, int> stoitoken_kinds_;
    std::unordered_map<int, std::string> itostoken_kinds_;

    std::string lexeme_;
    int state_{0};
    int last_match_{0};
    int line_{1};
    int line_start_{0};
    int column_{0};
    int column_start_{0};
    int backup_{0};
    int advance();
};

#endif //LEXER_H
