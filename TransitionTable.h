#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <map>
#include <string>

#include <Regexp.h>

class TransitionTable : private std::vector<std::array<int, 128>> {
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


using DFAState = std::vector<Regexp>;
using StateMap = std::map<DFAState, int>;
using AcceptMap = std::map<int, int>;

struct DFA {
    TransitionTable table;
    StateMap state_map;
    AcceptMap accept_map;
    int dead_state{-1};
    DFA() = default;
    DFA(DFA&&) = default;
    DFA& operator=(DFA&&) = default;
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;
};

DFA make_DFA(const std::vector<Regexp>& regex);
#endif //DFA_H
