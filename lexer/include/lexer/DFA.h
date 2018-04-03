#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <string>
#include <unordered_map>

struct DFA {
    using Index = int;
    using TransitionTable = std::vector<std::array<Index, 128> >;
    using AcceptMap = std::unordered_map<Index, Index>;

    TransitionTable table;
    AcceptMap accept_map;
    Index dead_state{-1};
};

class Regexp;

DFA make_DFA(const std::vector<Regexp>& regex);
#endif //DFA_H
