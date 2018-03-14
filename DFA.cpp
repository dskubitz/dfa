#include <vector>
#include <iostream>

#include "ASTNode.h"
#include "DFAFunctionCalculator.h"
#include "DFA.h"

using Dstate = boost::dynamic_bitset<>;

TransitionTable make_transition_table(DFAFunctionCalculator& calc)
{
    unsigned int state_num = 0;

    std::vector<Dstate> unmarked{calc.firstpos().at(calc.tree())};
    std::map<Dstate, unsigned int> dstates {
            {calc.firstpos().at(calc.tree()), state_num++}};

    TransitionTable table;
    table.add_state();

    while (!unmarked.empty()) {
        Dstate S = unmarked.back();
        unmarked.pop_back();

        for (auto& final : calc.acceptpos())
            if (S.test(final.first)) {
                table.add_final_state(dstates.at(S), final.second);
                break;
            }

        for (auto ch : alphabet) {
            Dstate U = make_bitset();
            for (size_t i = 0; i < CharNode::max_id(); ++i)
                if (S.test(i) && calc.symbols().at(i) == ch)
                    U |= calc.followpos().at(i);

            if (dstates.emplace(U, state_num).second) {
                state_num++;
                unmarked.emplace_back(U);
                table.add_state();
            }
            table[dstates.at(S)][ch] = dstates.at(U);
        }
    }

    return table;
}
