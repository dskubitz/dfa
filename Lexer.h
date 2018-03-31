#ifndef LEXER_H
#define LEXER_H

#include <tuple>
#include <istream>
#include <stdexcept>
#include <unordered_map>
#include <DFA.h>
#include <ostream>

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
};

class Lexer {
public:
    Lexer(DFA&& dfa, std::istream& i);

    int scan();

    const std::string& lexeme() const;
    const SourceLocation& current_position() const;
    const SourceLocation& lexeme_start_position() const;
    bool end_of_file();
    Lexer& reset_input_stream(std::istream& input);

private:
    DFA dfa_;
    std::reference_wrapper<std::istream> input_;

    std::string lexeme_;
    SourceLocation start_;
    SourceLocation current_;
    int last_match_{0};
    int backup_{0};
    bool encountered_eof_{false};

    int advance();
};

#endif //LEXER_H
