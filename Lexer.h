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
        return os << "line: " << location.line << " column: "
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

private:
    DFA dfa_;
    std::istream& input_;
    std::string lexeme_;
    int state_{0};
    int last_match_{0};
    SourceLocation start_;
    SourceLocation current_;
    int backup_{0};
    bool encountered_eof_{false};

    int advance();
    void set_line_column_start();
    int next_state(int c);
    void lookup_state();
};
#endif //LEXER_H
