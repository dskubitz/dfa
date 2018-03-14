#include "ASTNode.h"

size_t CharNode::num_ = 1;

CharNode::CharNode(char value)
        :value_(value),
         id_(num_++)
{
}

void CharNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

char CharNode::value() const { return value_; }

size_t CharNode::id() const { return id_; }

CharNode* CharNode::clone() const
{
    return new CharNode(*this);
}

size_t CharNode::max_id()
{
    return num_;
}

StarNode::StarNode(ASTNode* node)
        :expr_(node)
{
}

void StarNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

ASTNode* StarNode::expr() { return expr_; }

StarNode::~StarNode()
{
    delete expr_;
}

StarNode* StarNode::clone() const
{
    return new StarNode(expr_->clone());
}

CatNode::CatNode(ASTNode* left, ASTNode* right)
        :left_(left), right_(right)
{
}

void CatNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

ASTNode* CatNode::left() { return left_; }

ASTNode* CatNode::right() { return right_; }

CatNode::~CatNode()
{
    delete left_;
    delete right_;
}

CatNode* CatNode::clone() const
{
    return new CatNode(left_->clone(), right_->clone());
}

UnionNode::UnionNode(ASTNode* left, ASTNode* right)
        :left_(left), right_(right)
{
}

void UnionNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

ASTNode* UnionNode::left() { return left_; }

ASTNode* UnionNode::right() { return right_; }

UnionNode::~UnionNode()
{
    delete left_;
    delete right_;
}

UnionNode* UnionNode::clone() const
{
    return new UnionNode(left_->clone(), right_->clone());
}

void EpsilonNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

EpsilonNode* EpsilonNode::clone() const
{
    return new EpsilonNode(*this);
}

void EndmarkerNode::accept(ASTVisitor& v)
{
    v.visit(*this);
}

EndmarkerNode* EndmarkerNode::clone() const
{
    return new EndmarkerNode(*this);
}
