#include <vector>
#include <iostream>

#include "ASTNode.h"
#include "TreeFunctions.h"
#include "TransitionTable.h"

using Dstate = boost::dynamic_bitset<>;

void make_transition_table(const TreeFunctions& calc, TransitionTable& table)
{
    unsigned int state_num = 0;

    std::vector<Dstate> unmarked{calc.firstpos().at(calc.tree())};
    std::map<Dstate, unsigned int> dstates {
            {calc.firstpos().at(calc.tree()), state_num++}};

    table.add_state();

    while (!unmarked.empty()) {
        Dstate S = unmarked.back();
        unmarked.pop_back();

        for (auto& final : calc.acceptpos())
            if (S.test(final.first)) {
                table.final_states().emplace(dstates.at(S), final.second);
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
}

TransitionTable::TransitionTable(const TreeFunctions& calc)
{
    make_transition_table(calc, *this);
}
