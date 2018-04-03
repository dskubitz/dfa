#ifndef LEXER_H
#define LEXER_H

#include <tuple>
#include <stack>
#include <utility>
#include <istream>
#include <ostream>
#include <functional>

#include <lexer/DFA.h>

struct SourceLocation {
    int line;
    int column;

    SourceLocation(int l = {}, int c = {})
            : line(l), column(c) { }

    friend std::ostream&
    operator<<(std::ostream& os, const SourceLocation& location)
    {
        return os << location.line << ":"
                  << location.column;
    }

    friend bool operator==(const SourceLocation& lhs, const SourceLocation& rhs)
    {
        return std::tie(lhs.line, lhs.column) == std::tie(rhs.line, rhs.column);
    }

    friend bool operator!=(const SourceLocation& lhs, const SourceLocation& rhs)
    {
        return !(rhs == lhs);
    }
};

class Scanner {
public:
    Scanner(DFA&& dfa, std::istream& i);

    int scan();

    const std::string& lexeme() const;
    const SourceLocation& current_position() const;
    const SourceLocation& lexeme_start_position() const;
    bool end_of_file();
    Scanner& reset_input_stream(std::istream& input);

private:
    DFA dfa_;
    std::reference_wrapper<std::istream> input_;

    std::string lexeme_;
    SourceLocation start_;
    SourceLocation current_;
    std::stack<int, std::vector<int>> previous_line_length_;
    int last_match_{0};
    int backup_{0};
    bool encountered_eof_{false};

    int advance();
    void retract();
};
#endif //LEXER_H
