#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <unordered_map>
#include <string>

class TreeFunctions;
using state_type = std::array<int, 128>;

class TransitionTable : private std::vector<state_type> {
    using base = std::vector<state_type>;
    std::unordered_map<size_type, std::string> final_states_;
public:
    using base::vector;
    using base::operator[];
    using base::at;
    using base::begin;
    using base::end;
    using base::size;
    using base::capacity;
    using base::reserve;
    using base::resize;

    explicit TransitionTable(const TreeFunctions& calc);

    /**
     * Add a new state to the table and return its index.
     */
    base::size_type add_state()
    {
        this->emplace_back();
        return this->size() - 1;
    }

    std::unordered_map<size_type, std::string>&
    final_states() noexcept { return final_states_; }

    const std::unordered_map<size_type, std::string>&
    final_states() const noexcept { return final_states_; }
};

TransitionTable make_transition_table(const TreeFunctions& calc);
void make_transition_table(const TreeFunctions& calc, TransitionTable& table);

#endif //DFA_H
