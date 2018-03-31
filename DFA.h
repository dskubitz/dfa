#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <map>
#include <string>

#include <Regexp.h>
#include <unordered_map>

struct DFA {
    using Index = int;
    using TransitionTable = std::vector<std::array<Index, 128> >;
    using State = std::vector<Regexp>;
    using StateMap = std::map<State, Index>;
    using AcceptMap = std::unordered_map<Index, Index>;

    TransitionTable table;
    StateMap state_map;
    AcceptMap accept_map;
    Index dead_state{-1};

    DFA() = default;
    DFA(DFA&&) = default;
    DFA& operator=(DFA&&) = default;
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;
};

DFA make_DFA(const std::vector<Regexp>& regex);
#endif //DFA_H
