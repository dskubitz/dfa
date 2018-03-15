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
    explicit TreeFunctions(ASTNode* re);

    void visit(StarNode& node) override;
    void visit(CatNode& node) override;
    void visit(UnionNode& node) override;
    void visit(CharNode& node) override;
    void visit(EpsilonNode& node) override;
    void visit(EndmarkerNode& node) override;

    ASTNode* tree() noexcept { return tree_; }

    const ASTNode* tree() const noexcept { return tree_; }

    std::unordered_map<const ASTNode*, bool>&
    nullable() noexcept { return nullable_; };

    const std::unordered_map<const ASTNode*, bool>&
    nullable() const noexcept { return nullable_; };

    const std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    firstpos() const noexcept { return firstpos_; };

    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    firstpos() noexcept { return firstpos_; };

    const std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    lastpos() const noexcept { return lastpos_; };

    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>>&
    lastpos() noexcept { return lastpos_; };

    const std::vector<boost::dynamic_bitset<>>&
    followpos() const noexcept { return followpos_; };

    std::vector<boost::dynamic_bitset<>>&
    followpos() noexcept { return followpos_; };

    const std::vector<char> symbols() const noexcept { return symbols_; }

    std::vector<char> symbols() noexcept { return symbols_; }

    std::map<unsigned int, std::string>& acceptpos() noexcept { return acceptpos_; }

    const std::map<unsigned int, std::string>&
    acceptpos() const noexcept { return acceptpos_; }

private:
    ASTNode* tree_;
    std::unordered_map<const ASTNode*, bool> nullable_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> firstpos_;
    std::unordered_map<const ASTNode*, boost::dynamic_bitset<>> lastpos_;
    std::vector<boost::dynamic_bitset<>> followpos_;
    std::vector<char> symbols_;
    std::map<unsigned int, std::string> acceptpos_;
};

#endif //LEXER_DFAFUNCTIONCALCULATOR_H