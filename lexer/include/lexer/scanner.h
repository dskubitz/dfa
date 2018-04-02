#ifndef LEXER_H
#define LEXER_H

#include <tuple>
#include <stack>
#include <utility>
#include <istream>
#include <ostream>
#include <functional>

#include <lexer/dfa.h>

struct source_location {
    int line;
    int column;

    source_location(int l = {}, int c = {})
            : line(l), column(c) { }

    friend std::ostream&
    operator<<(std::ostream& os, const source_location& location)
    {
        return os << location.line << ":"
                  << location.column;
    }
};

class scanner {
public:
    scanner(dfa&& dfa, std::istream& i);

    int scan();

    const std::string& lexeme() const;
    const source_location& current_position() const;
    const source_location& lexeme_start_position() const;
    bool end_of_file();
    scanner& reset_input_stream(std::istream& input);

private:
    dfa dfa_;
    std::reference_wrapper<std::istream> input_;

    std::string lexeme_;
    source_location start_;
    source_location current_;
    std::stack<int, std::vector<int>> previous_line_length_;
    int last_match_{0};
    int backup_{0};
    bool encountered_eof_{false};

    int advance();
    void retract();
};
#endif //LEXER_H
