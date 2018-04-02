#include <lexer/lexer.h>
#include <iostream>

lexer::lexer(dfa&& dfa, std::istream& i)
        : dfa_(std::move(dfa)), input_(i), current_(1, 0)
{
}

int lexer::scan()
{
    if (encountered_eof_)
        return dfa_.accept_map.at(last_match_);
    auto& input = input_.get();

    lexeme_.clear();

    int c = advance();
    int state = dfa_.table[0][c];

    start_ = current_;

    while (input.good()) {
        lexeme_.push_back(static_cast<char>(c));

        auto it = dfa_.accept_map.find(state);
        if (it != dfa_.accept_map.end()) {
            backup_ = 0;
            last_match_ = state;
        } else {
            backup_ -= 1;
        }

        int nextc = input.peek();
        if (nextc == EOF) {
            encountered_eof_ = true;
            return dfa_.accept_map.at(last_match_);
        }

        int next_ = dfa_.table[state][nextc];
        if (next_ == dfa_.dead_state)
            break;

        c = advance();
        state = next_;
    }

    retract();
    return dfa_.accept_map.at(last_match_);
}

int lexer::advance()
{
    int c = input_.get().get();
    if (c == '\n') {
        ++current_.line;
        previous_line_length_.push(current_.column);
        current_.column = 0;
    } else {
        ++current_.column;
    }
    return c;
}

void lexer::retract()
{
    auto& input = input_.get();

    input.seekg(backup_, std::ios_base::cur);
    while (backup_++ < 0) {
        int c = lexeme_.back();
        lexeme_.pop_back();
        if (c == '\n' || current_.column == 0) {
            --current_.line;
            current_.column = previous_line_length_.top();
        } else {
            --current_.column;
        }
    }
}

const std::string& lexer::lexeme() const
{
    return lexeme_;
}

const source_location& lexer::lexeme_start_position() const
{
    return start_;
}

const source_location& lexer::current_position() const
{
    return current_;
}

bool lexer::end_of_file()
{
    return encountered_eof_;
}

lexer& lexer::reset_input_stream(std::istream& input)
{
    input_ = input;
    start_ = current_ = {1, 0};
    encountered_eof_ = false;
    last_match_ = {};
    backup_ = {};

    return *this;
}



