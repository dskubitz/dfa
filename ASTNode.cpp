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

//@formatter:off
bool CharNode::operator==(const ASTNode& node) const { return node == *this; }
bool CharNode::operator==(const StarNode& node) const { return false; }
bool CharNode::operator==(const CatNode& node) const { return false; }
bool CharNode::operator==(const UnionNode& node) const { return false; }
bool CharNode::operator==(const EpsilonNode& node) const { return false; }
bool CharNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool CharNode::operator==(const CharNode& node) const
{ return value_ == node.value_; }

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

//@formatter:off
bool StarNode::operator==(const ASTNode& node) const { return node == *this; }
bool StarNode::operator==(const CatNode& node) const { return false; }
bool StarNode::operator==(const UnionNode& node) const { return false; }
bool StarNode::operator==(const CharNode& node) const { return false; }
bool StarNode::operator==(const EpsilonNode& node) const { return false; }
bool StarNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool StarNode::operator==(const StarNode& node) const
{ return expr_->operator==(*node.expr_); }

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

//@formatter:off
bool CatNode::operator==(const ASTNode& node) const { return node == *this; }
bool CatNode::operator==(const StarNode& node) const { return false; }
bool CatNode::operator==(const UnionNode& node) const { return false; }
bool CatNode::operator==(const CharNode& node) const { return false; }
bool CatNode::operator==(const EpsilonNode& node) const { return false; }
bool CatNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool CatNode::operator==(const CatNode& node) const
{ return left_->operator==(*node.left_) && right_->operator==(*node.right_); }

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

//@formatter:off
bool UnionNode::operator==(const ASTNode& node) const { return node == *this; }
bool UnionNode::operator==(const StarNode& node) const { return false; }
bool UnionNode::operator==(const CatNode& node) const { return false; }
bool UnionNode::operator==(const CharNode& node) const { return false; }
bool UnionNode::operator==(const EpsilonNode& node) const { return false; }
bool UnionNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool UnionNode::operator==(const UnionNode& node) const
{ return left_->operator==(*node.left_) && right_->operator==(*node.right_); }

void EpsilonNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

EpsilonNode* EpsilonNode::clone() const
{
    return new EpsilonNode;
}

//@formatter:off
bool EpsilonNode::operator==(const ASTNode& node) const { return node == *this; }
bool EpsilonNode::operator==(const StarNode& node) const { return false; }
bool EpsilonNode::operator==(const CatNode& node) const { return false; }
bool EpsilonNode::operator==(const UnionNode& node) const { return false; }
bool EpsilonNode::operator==(const CharNode& node) const { return false; }
bool EpsilonNode::operator==(const EpsilonNode& node) const { return true; }
bool EpsilonNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

void EmptyNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

ASTNode* EmptyNode::clone() const
{
    return new EmptyNode;
}

//@formatter:off
bool EmptyNode::operator==(const ASTNode& node) const { return node == *this; }
bool EmptyNode::operator==(const StarNode& node) const { return false; }
bool EmptyNode::operator==(const CatNode& node) const { return false; }
bool EmptyNode::operator==(const UnionNode& node) const { return false; }
bool EmptyNode::operator==(const CharNode& node) const { return false; }
bool EmptyNode::operator==(const EpsilonNode& node) const { return false; }
bool EmptyNode::operator==(const EmptyNode& node) const { return true; }
//@formatter:on

