#ifndef DFA_H
#define DFA_H

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
};

TransitionTable make_transition_table(ASTNode& node);

#endif //DFA_H
