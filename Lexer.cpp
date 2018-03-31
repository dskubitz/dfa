#include <Lexer.h>
#include <iostream>

Lexer::Lexer(DFA&& dfa, std::istream& i)
        : dfa_(std::move(dfa)), input_(i), current_(1, 0) { }

int Lexer::scan()
{
    auto& input = input_.get();
    if (encountered_eof_)
        return dfa_.accept_map.at(last_match_);

    lexeme_.clear();

    int c = advance();
    int state = dfa_.table[0][c];

    start_ = current_;

    while (input.good()) {
        //
        lexeme_.push_back(static_cast<char>(c));
        //
        auto it = dfa_.accept_map.find(state);
        if (it != dfa_.accept_map.end()) {
            backup_ = 0;
            last_match_ = state;
        } else {
            backup_ -= 1;
        }
        //
        int nextc = input.peek();
        if (nextc == EOF) {
            encountered_eof_ = true;
            return dfa_.accept_map.at(last_match_);
        }

        //
        int next_ = dfa_.table[state][nextc];
        if (next_ == dfa_.dead_state)
            break;

        c = advance();
        state = next_;
    }

    input.seekg(backup_, std::ios_base::cur);
    return dfa_.accept_map.at(last_match_);
}

int Lexer::advance()
{
    int c = input_.get().get();
    if (c == '\n') {
        ++current_.line;
        current_.column = 0;
    } else {
        ++current_.column;
    }
    return c;
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

Lexer& Lexer::reset_input_stream(std::istream& input)
{
    input_ = input;
    start_ = current_ = {1, 0};
    encountered_eof_ = false;
    last_match_ = {};
    backup_ = {};

    return *this;
}

