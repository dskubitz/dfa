#include "ASTNode.h"

size_t CharNode::num_ = 0;

CharNode::CharNode(char value)
        :value_(value),
         id_(num_++)
{
}

void CharNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

char CharNode::value() const { return value_; }

size_t CharNode::id() const { return id_; }

CharNode* CharNode::clone() const
{
    return new CharNode(value_);
}

size_t CharNode::max_id()
{
    return num_;
}

StarNode::StarNode(ASTNode* node)
        :expr_(node)
{
}

void StarNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

const ASTNode* StarNode::expr() const { return expr_; }

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

void CatNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

const ASTNode* CatNode::left() const { return left_; }

const ASTNode* CatNode::right() const { return right_; }

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

void UnionNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

const ASTNode* UnionNode::left()const { return left_; }

const ASTNode* UnionNode::right()const { return right_; }

UnionNode::~UnionNode()
{
    delete left_;
    delete right_;
}

UnionNode* UnionNode::clone() const
{
    return new UnionNode(left_->clone(), right_->clone());
}

void EpsilonNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

EpsilonNode* EpsilonNode::clone() const
{
    return new EpsilonNode;
}

void EndmarkerNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

EndmarkerNode* EndmarkerNode::clone() const
{
    return new EndmarkerNode(*this);
}

EndmarkerNode::EndmarkerNode(std::string name)
        :CharNode('#'), name_(std::move(name)) { }

const std::string& EndmarkerNode::name() const noexcept { return name_; }
