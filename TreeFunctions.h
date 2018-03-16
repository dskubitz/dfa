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
    return boost::dynamic_bitset<>(CharNode::max_id());
}

inline boost::dynamic_bitset<> make_bitset(size_t pos)
{
    boost::dynamic_bitset<> bitset(CharNode::max_id());
    bitset.set(pos);
    return bitset;
}

class TreeFunctions : public ASTVisitor {
public:
    explicit TreeFunctions(const ASTNode* re);

    void visit(const StarNode* node) override;
    void visit(const CatNode* node) override;
    void visit(const UnionNode* node) override;
    void visit(const CharNode* node) override;
    void visit(const EpsilonNode* node) override;
    void visit(const EndmarkerNode* node) override;
    void visit(const ASTNode* node) override;

    const std::unordered_map<const ASTNode*, bool>& nullable() const noexcept
    { return nullable_; }

    const std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    firstpos() const noexcept { return firstpos_; };

    const std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    lastpos() const noexcept { return lastpos_; };

    const std::vector<boost::dynamic_bitset<>>&
    followpos() const noexcept { return followpos_; };

    const std::map<unsigned int, std::string>&
    acceptpos() const noexcept { return acceptpos_; }

    const ASTNode* tree() const noexcept { return tree_; }

    const std::vector<char> symbols() const noexcept { return symbols_; }

private:
    const ASTNode* tree_;
    std::unordered_map<const ASTNode*, bool> nullable_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> firstpos_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> lastpos_;
    std::vector<boost::dynamic_bitset<>> followpos_;
    std::vector<char> symbols_;
    std::map<unsigned int, std::string> acceptpos_;
};

#endif //LEXER_DFAFUNCTIONCALCULATOR_H
