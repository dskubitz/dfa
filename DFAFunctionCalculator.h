#ifndef LEXER_DFAFUNCTIONCALCULATOR_H
#define LEXER_DFAFUNCTIONCALCULATOR_H

#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <boost/dynamic_bitset.hpp>

#include "ASTNode.h"

inline boost::dynamic_bitset<> make_bitset()
{
    return boost::dynamic_bitset<> (CharNode::max_id());
}

inline boost::dynamic_bitset<> make_bitset(size_t pos)
{
    boost::dynamic_bitset<> bitset(CharNode::max_id());
    bitset.set(pos);
    return bitset;
}

class DFAFunctionCalculator : public ASTVisitor {
public:
    explicit DFAFunctionCalculator(ASTNode& re);

    void visit(StarNode& node) override;
    void visit(CatNode& node) override;
    void visit(UnionNode& node) override;
    void visit(CharNode& node) override;
    void visit(EpsilonNode& node) override;
    void visit(EndmarkerNode& node) override;

    ASTNode& tree_;

    std::unordered_map<const ASTNode*, bool> nullable_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> firstpos_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> lastpos_;
    std::vector<boost::dynamic_bitset<>> followpos_;
    std::vector<char> symbols_;
private:
};
#endif //LEXER_DFAFUNCTIONCALCULATOR_H
