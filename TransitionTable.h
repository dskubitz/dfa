#ifndef DFA_H
#define DFA_H

#include <Regexp.h>


#include <vector>
#include <array>
#include <map>
#include <string>

class TreeFunctions;
class ASTNode;

using state_type = std::array<int, 128>;

class TransitionTable : private std::vector<state_type> {
    using base = std::vector<state_type>;
public:
    using base::vector;
    using base::operator[];
    using base::at;
    using base::begin;
    using base::end;
    using base::size;

    void add_state() { this->emplace_back(); }
};

TransitionTable make_transition_table(const Regexp& regex);
TransitionTable make_transition_table(const std::vector<Regexp>& regex);
#endif //DFA_H
