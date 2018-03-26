#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <map>
#include <string>

#include <Regexp.h>

// Transition function δ : Q × Σ → Q
class TransitionFunction : private std::vector<std::array<int, 128>> {
    using base = std::vector<std::array<int, 128>>;
public:
    using base::vector;
    using base::operator[];
    using base::at;
    using base::begin;
    using base::end;
    using base::size;

    void add_state() { this->emplace_back(); }
};


// States of the DFA are regular vectors
using DFAState = std::vector<Regexp>;
// The set of states (Q) and their index in δ
using StateMap = std::map<DFAState, int>;
// Alphabet Σ is implied
// q0 is index 0
using DFA = std::pair<TransitionFunction, StateMap>;

DFA make_DFA(const std::vector<Regexp>& regex);
#endif //DFA_H
