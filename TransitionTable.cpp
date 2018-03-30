#include <TransitionTable.h>
#include <Derivative.h>
#include <DerivativeClass.h>
#include <iostream>

namespace {
std::tuple<bool, bool, int> accepting(const DFAState& state)
{
    Nullable N;

    int index = 0;
    int nempty = 0;
    for (const auto& i : state) {
        if (N.evaluate(i)) {
            return {true, false, index};
        }
        if (i->type() == RegexNode::Type::Empty)
            ++nempty;
        ++index;
    }

    return {false, index == nempty, index};
}
}

DFA make_DFA(const std::vector<Regexp>& regex)
{
    DFA res;
    TransitionTable& table = res.table;
    StateMap& dstates = res.state_map;
    AcceptMap& accepts = res.accept_map;
    int& dead_state_index = res.dead_state;

    dstates.insert({regex, 0});

    Derivative deriv;
    DerivativeClass derivativeClass;

    std::vector<DFAState> unmarked{regex};
    size_t num = 1;
    table.add_state();

    while (!unmarked.empty()) {
        DFAState from = unmarked.back();
        unmarked.pop_back();
        std::unordered_set<Bitset> dclass = make_derivative_class(from);

        bool is_accepting;
        bool is_dead;
        int vector_index;
        std::tie(is_accepting, is_dead, vector_index) = accepting(from);
        if (is_accepting)
            accepts.emplace(dstates.at(from), vector_index);
        else if (is_dead)
            dead_state_index = dstates.at(from);

        for (auto& set : dclass) {
            auto c = static_cast<char>(first_occurring(set));
            DFAState to = make_derivative(from, c);

            for (unsigned i = 0; i < set.size(); ++i) {
                if (!set.test(i))
                    continue;

                if (dstates.emplace(to, num).second) {
                    ++num;
                    unmarked.push_back(to);
                    table.add_state();
                }
                table[dstates.at(from)][i] = static_cast<int>(dstates.at(to));
            }
        }
    }


    return res;
}



