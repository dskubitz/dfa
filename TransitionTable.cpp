#include <TransitionTable.h>
#include <unordered_map>
#include <Derivative.h>
#include <iostream>
#include "DerivativeClass.h"

TransitionTable make_transition_table(const std::vector<Regex>& regex)
{
    using State = std::vector<Regex>;

    Derivative deriv;
    DerivativeClass derivativeClass;

    std::vector<State> unmarked{regex};
    std::map<State, size_t> dstates{{regex, 0}};
    size_t num = 1;
    TransitionTable table;
    table.add_state();

    while (!unmarked.empty()) {
        State from = unmarked.back();
        unmarked.pop_back();
        std::unordered_set<Bitset> dclass = make_derivative_class(from);

        for (auto& set : dclass) {
            auto c = static_cast<char>(first(set));
            State to = make_derivative(from, c);

            for (int i = 0; i < set.size(); ++i) {
                if (!set.test(static_cast<unsigned>(i)))
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
    for (auto& i : dstates) {
        std::cout << i.second << ' ';
        print(i.first);
    }

    return table;
}

TransitionTable make_transition_table(const Regex& regex)
{
    Derivative deriv;
    DerivativeClass derivativeClass;

    std::vector<Regex> unmarked{regex};
    std::map<Regex, size_t> dstates{{regex, 0}};
    size_t num = 1;
    TransitionTable table;
    table.add_state();

    while (!unmarked.empty()) {
        Regex from = unmarked.back();
        unmarked.pop_back();
        std::unordered_set<Bitset> dclass = derivativeClass.evaluate(from);

        for (auto& set : dclass) {
            auto c = static_cast<char>(first(set));
            Regex to = deriv.derive(from, c);

            for (int i = 0; i < set.size(); ++i) {
                if (!set.test(static_cast<unsigned>(i)))
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

    return table;
}
