#include <TransitionTable.h>
#include <unordered_map>
#include <Derivative.h>
#include <iostream>

/*
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
    std::map<Dstate, unsigned int> dstates{
            {calc.firstpos().at(calc.tree()), state_num++}};

    table.add_state();
    table.final_states().emplace(0, "start");
    table.final_states().emplace(1, "dead");

    while (!unmarked.empty()) {
        Dstate S = unmarked.back();
        unmarked.pop_back();

        for (auto& final : calc.acceptpos())
            if (S.test(final.first)) {
                table.final_states().emplace(dstates.at(S), final.second);
                break;
            }

        for (int ch = 0; ch < 128; ++ch) {
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

TransitionTable make_transition_table(const ASTNode* ast)
{
    return TransitionTable(TreeFunctions(ast));
}
 */
TransitionTable make_transition_table(const Regex& regex)
{
    Derivative deriv;
    std::vector<Regex> unmarked{regex};
    std::map<Regex, size_t> dstates{{regex, 0}};
    size_t num = 1;
    TransitionTable table;
    table.add_state();

    while (!unmarked.empty()) {
        Regex from = unmarked.back();
        unmarked.pop_back();

        for (auto c : alphabet) {
            Regex to = deriv.derive(from, c);

            if (dstates.emplace(to, num).second) {
                if (isprint(c))
                    std::cout << char(c) << '\n';
                else std::cout << c << '\n';
                ++num;
                unmarked.push_back(to);
                table.add_state();
            }
            table[dstates.at(from)][c] = dstates.at(to);
        }
    }
    for (auto& i : dstates) {
        std::cout << i.first.get()->to_string() << ' ' << i.second << '\n';
    }

    return table;
}
