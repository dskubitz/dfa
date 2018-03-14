#ifndef DFA_H
#define DFA_H

#include "TransitionTable.h"

class DFA {

    TransitionTable table_;
};

void print_pos_sets(ASTNode& tree);

#endif //DFA_H
