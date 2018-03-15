#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <unordered_map>
#include <string>

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

    /**
     * Add a new state to the table and return its index.
     */
    base::size_type add_state()
    {
        this->emplace_back();
        return this->size() - 1;
    }

    void add_final_state(size_type index, const std::string& str)
    {
        final_states_.emplace(index, str);
    }

    auto find_final(size_type index) -> decltype(final_states_.find(index))
    {
        return final_states_.find(index);
    }

    auto final_states_end() -> decltype(final_states_.end())
    {
        return final_states_.end();
    }

    const auto
    find_final(size_type index) const -> decltype(final_states_.find(index))
    {
        return final_states_.find(index);
    }

    const auto final_states_end() const -> decltype(final_states_.end())
    {
        return final_states_.end();
    }
};

class TreeFunctions;

TransitionTable
make_transition_table(TreeFunctions& calc);

#endif //DFA_H
