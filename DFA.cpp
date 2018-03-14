#include <vector>
#include <iostream>

#include "ASTNode.h"
#include "DFAFunctionCalculator.h"
#include "DFA.h"

using Dstate = boost::dynamic_bitset<>;

TransitionTable make_transition_table(ASTNode* node)
{
    DFAFunctionCalculator calc(node);
    int state_num = 0;

    std::vector<Dstate> stack {calc.firstpos().at(node)};

    std::map<Dstate, int> unseen {{calc.firstpos().at(node), state_num++}};
    std::map<Dstate, int> seen;

    TransitionTable table;
    table.add_state();

    while (!stack.empty()) {
        Dstate S = stack.back();
        stack.pop_back();
        seen.emplace(S, unseen.at(S));

        for (auto& final : calc.acceptpos()) {
            if (S.test(final.first)) {
                std::cout << seen.at(S) << ' ' << final.second << '\n';
                break;
            }
        }

        for (auto ch : alphabet) {
            Dstate U = make_bitset();
            for (size_t i = 0; i < CharNode::max_id(); ++i) {
                if (S.test(i) && calc.symbols().at(i) == ch) {
                    U |= calc.followpos().at(i);
                }
            }
            auto it = seen.find(U);
            if (it == seen.end()) {
                if (unseen.emplace(U, state_num).second) {
                    state_num++;
                    stack.emplace_back(U);
                    table.add_state();
                }
            }
            table[seen.at(S)][ch] = unseen.at(U);
        }
    }

    return table;
}
