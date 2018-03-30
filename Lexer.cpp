#include <Lexer.h>
#include <iostream>

Lexer::Lexer(DFA&& dfa, std::istream& i)
        : dfa_(std::move(dfa)), input_(i), current_(1, 0) { }

int Lexer::scan()
{
    if (encountered_eof_)
        return dfa_.accept_map.at(last_match_);
    // clear previous lexeme
    lexeme_.clear();

    int c = advance();
    set_line_column_start();

    state_ = next_state(c);

    while (input_.good()) {
        lexeme_.push_back(static_cast<char>(c));

        lookup_state();

        int nextc = input_.peek();
        if (nextc == EOF) {
            encountered_eof_ = true;
            return dfa_.accept_map.at(last_match_);
        }

        int next_ = next_state(nextc);

        if (next_ == dfa_.dead_state) break;

        c = advance();
        state_ = next_;
    }
    input_.seekg(backup_, std::ios_base::cur);
    state_ = 0;
    return dfa_.accept_map.at(last_match_);
}

int Lexer::advance()
{
    int c = input_.get();
    if (c == '\n') {
        ++current_.line;
        current_.column = 0;
    } else {
        ++current_.column;
    }
    return c;
}

void Lexer::set_line_column_start()
{
    start_ = current_;
}

int Lexer::next_state(int c)
{
    return dfa_.table[state_][c];
}

void Lexer::lookup_state()
{
    auto it = dfa_.accept_map.find(state_);
    if (it != dfa_.accept_map.end()) {
        backup_ = 0;
        last_match_ = state_;
    } else {
        backup_ -= 1;
    }
}

const std::string& Lexer::lexeme() const
{
    return lexeme_;
}

const SourceLocation& Lexer::lexeme_start_position() const
{
    return start_;
}

const SourceLocation& Lexer::current_position() const
{
    return current_;
}

bool Lexer::end_of_file()
{
    return encountered_eof_;
}
