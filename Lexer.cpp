//
// Created by devan on 3/16/18.
//

#include <Lexer.h>
#include <TransitionTable.h>
#include <iostream>

Lexer::Lexer(const TransitionTable& t, std::istream& i)
        : table_(t), input_(i)
{
    if (!input_)
        throw LexerError("bad input");
    int n = 0;
    for (auto& accept : table_.final_states()) {
        stoitoken_kinds_[accept.second] = n;
        itostoken_kinds_[n++] = accept.second;
    }
    stoitoken_kinds_["EOF"] = EOF;
    itostoken_kinds_[-1] = "EOF";
}

Token Lexer::scan()
{
    // clear previous lexeme
    lexeme_.clear();

    int c = advance();

    line_start_ = line_;
    column_start_ = column_;

    state_ = table_[state_][c];

    while (input_.good()) {
        std::cout << char(c) << ' ' << state_ << " ";

        lexeme_.push_back(static_cast<char>(c));

        // If we landed on a final state, remember its position
        auto it = table_.final_states().find(state_);
        if (it != table_.final_states().end()) {
            backup_ = 0;
            last_match_ = state_;
        } else {
            backup_ -= 1;
        }

        // Peek to the next state
        int nextc = input_.peek();
        std::cout << char(nextc) << ' ';
        if (nextc == EOF)
            break;

        int next_state = table_[state_][nextc];
        std::cout << next_state << '\n';

        // If next state id the dead state
        if (next_state == 1)
            break;

        c = advance();
        state_ = next_state;
    }
    int type = stoitoken_kinds_.at(table().final_states().at(last_match_));
    input_.seekg(backup_, std::ios_base::cur);
    state_ = 0;
    return Token{lexeme_, type, line_start_, column_start_};
}

int Lexer::advance()
{
    int c = input_.get();
    if (c == '\n') {
        ++line_;
        column_ = 0;
    } else {
        ++column_;
    }
    return c;
}
