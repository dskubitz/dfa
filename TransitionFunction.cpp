#include <TransitionFunction.h>
#include <Derivative.h>
#include <DerivativeClass.h>

#include <unordered_map>
#include <iostream>

DFA make_DFA(const std::vector<Regexp>& regex)
{
    DFA res;
    TransitionFunction& table = res.first;
    StateMap& dstates = res.second;

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
        std::cout << dstates.at(from) << '\n';
        for (auto& i : dclass) {
            std::cout << i.to_string() << '\n';
        }

        for (auto& set : dclass) {
            auto c = static_cast<char>(first_occurring(set));
            DFAState to = make_derivative(from, c);

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
        std::cout << '\n';
    }

    return res;
}
