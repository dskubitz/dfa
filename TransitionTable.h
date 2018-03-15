#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <map>
#include <string>

class TreeFunctions;

using state_type = std::array<int, 128>;

class TransitionTable : private std::vector<state_type> {
    using base = std::vector<state_type>;
    std::map<size_type, std::string> final_states_;
public:
    using base::operator[];
    using base::at;
    using base::begin;
    using base::end;
    using base::size;

    explicit TransitionTable(const TreeFunctions& calc);

    /**
     * Add a new state to the table and return its index.
     */
    void add_state() { this->emplace_back(); }

    std::map<size_type, std::string>&
    final_states() noexcept { return final_states_; }

    const std::map<size_type, std::string>&
    final_states() const noexcept { return final_states_; }
};

void make_transition_table(const TreeFunctions& calc, TransitionTable& table);

#endif //DFA_H
