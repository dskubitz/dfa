#include <iostream>
#include <vector>

#include "ASTNode.h"
#include "DFAFunctionCalculator.h"
#include "DFA.h"

void print_pos_sets(ASTNode& tree)
{
    DFAFunctionCalculator n(tree);

    /*
    std::cout << "digraph {\n\trankdir=\"LR\"\n\tsize=\"8,5\"\n";
    for (auto& i : n.followpos_) {
        std::cout << '\t' << i.first << " -> {";
        for (auto it = i.second.begin(), end = i.second.end(); it != end;) {
            std::cout << *it++;
            if (it == end)
                break;
            std::cout << ' ';
        }
        std::cout << "};\n";
    }
    std::cout << "}\n";
    */
}
