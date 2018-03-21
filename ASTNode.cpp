#include "ASTNode.h"

size_t CharNode::num_ = 0;

CharNode::CharNode(char value)
        : value_(value),
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
bool CharNode::operator==(const IntersectionNode& node) const { return false; }
bool CharNode::operator==(const ComplementNode& node) const { return false; }
//@formatter:on

bool CharNode::operator==(const CharNode& node) const
{
    return value_ == node.value_;
}

StarNode::StarNode(ASTNode* node)
        : expr_(node)
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
bool StarNode::operator==(const IntersectionNode& node) const { return false; }
bool StarNode::operator==(const ComplementNode& node) const { return false; }
bool StarNode::operator==(const EpsilonNode& node) const { return false; }
bool StarNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool StarNode::operator==(const StarNode& node) const
{
    return expr_->operator==(*node.expr_);
}

CatNode::CatNode(ASTNode* left, ASTNode* right)
        : left_(left), right_(right)
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
bool CatNode::operator==(const IntersectionNode& node) const { return false; }
bool CatNode::operator==(const ComplementNode& node) const { return false; }
//@formatter:on

bool CatNode::operator==(const CatNode& node) const
{
    return left_->operator==(*node.left_) && right_->operator==(*node.right_);
}

UnionNode::UnionNode(ASTNode* left, ASTNode* right)
        : left_(left), right_(right)
{
}

void UnionNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

const ASTNode* UnionNode::left() const { return left_; }

const ASTNode* UnionNode::right() const { return right_; }

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
bool UnionNode::operator==(const IntersectionNode& node) const { return false; }
bool UnionNode::operator==(const ComplementNode& node) const { return false; }
//@formatter:on

bool UnionNode::operator==(const UnionNode& node) const
{
    return left_->operator==(*node.left_) && right_->operator==(*node.right_);
}

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
bool EpsilonNode::operator==(const IntersectionNode& node) const { return false; }
bool EpsilonNode::operator==(const ComplementNode& node) const { return false; }
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
bool EmptyNode::operator==(const IntersectionNode& node) const { return false; }
bool EmptyNode::operator==(const ComplementNode& node) const { return false; }
//@formatter:on

void IntersectionNode::accept(ASTVisitor* v) const { v->visit(this); }

IntersectionNode* IntersectionNode::clone() const
{
    return new IntersectionNode(left_->clone(), right_->clone());
}

//@formatter:off
bool IntersectionNode::operator==(const ASTNode& node) const { return node == *this; }
bool IntersectionNode::operator==(const StarNode& node) const { return false; }
bool IntersectionNode::operator==(const CatNode& node) const { return false; }
bool IntersectionNode::operator==(const UnionNode& node) const { return false; }
bool IntersectionNode::operator==(const CharNode& node) const { return false; }
bool IntersectionNode::operator==(const EpsilonNode& node) const { return false; }
bool IntersectionNode::operator==(const ComplementNode& node) const { return false; }
bool IntersectionNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on

bool IntersectionNode::operator==(const IntersectionNode& node) const
{
    return left_->operator==(*node.left_) && right_->operator==(*node.right_);
}

IntersectionNode::~IntersectionNode()
{
    delete left_;
    delete right_;
}

IntersectionNode::IntersectionNode(ASTNode* left, ASTNode* right)
        : left_(left), right_(right)
{
}

const ASTNode* IntersectionNode::left() const { return left_; }

const ASTNode* IntersectionNode::right() const { return right_; }

void ComplementNode::accept(ASTVisitor* v) const { v->visit(this); }

ComplementNode* ComplementNode::clone() const
{
    return new ComplementNode(expr_->clone());
}

//@formatter:off
bool ComplementNode::operator==(const ASTNode& node) const { return node == *this; }
bool ComplementNode::operator==(const StarNode& node) const { return false; }
bool ComplementNode::operator==(const CatNode& node) const { return false; }
bool ComplementNode::operator==(const UnionNode& node) const { return false; }
bool ComplementNode::operator==(const IntersectionNode& node) const { return false; }
bool ComplementNode::operator==(const CharNode& node) const { return false; }
bool ComplementNode::operator==(const EpsilonNode& node) const { return false; }
bool ComplementNode::operator==(const EmptyNode& node) const { return false; }
//@formatter:on
bool ComplementNode::operator==(const ComplementNode& node) const
{
    return expr_->operator==(*node.expr_);
}

ComplementNode::~ComplementNode() { delete expr_; }

ComplementNode::ComplementNode(ASTNode* expr)
        : expr_(expr) { }

const ASTNode* ComplementNode::expr() const { return expr_; }

ASTNode* make_union(ASTNode* left, ASTNode* right)
{
    if (dynamic_cast<EmptyNode*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<EmptyNode*>(right)) {
        delete right;
        return left;
    } else if (*left == *right) {
        delete right;
        return left;
    } else {
        return new UnionNode(left, right);
    }
}

ASTNode* make_cat(ASTNode* left, ASTNode* right)
{
    if (dynamic_cast<EmptyNode*>(left) || dynamic_cast<EmptyNode*>(right)) {
        delete right;
        return left;
    } else if (dynamic_cast<EpsilonNode*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<EpsilonNode*>(right)) {
        delete right;
        return left;
    } else {
        return new CatNode(left, right);
    }
}

ASTNode* make_star(ASTNode* expr)
{
    if (dynamic_cast<EmptyNode*>(expr)) {
        delete expr;
        return new EpsilonNode;
    } else if (dynamic_cast<EpsilonNode*>(expr)) {
        return expr;
    } else if (auto p = dynamic_cast<StarNode*>(expr)) {
        ASTNode* q = p->expr()->clone();
        delete p;
        return make_star(q);
    } else {
        return new StarNode(expr);
    }
}

ASTNode* make_intersection(ASTNode* left, ASTNode* right)
{
    if (dynamic_cast<EmptyNode*>(left)) {
        delete right;
        return left;
    } else if (dynamic_cast<EmptyNode*>(right)) {
        delete left;
        return right;
    } else if (*left == *right) {
        delete right;
        return left;
    } else {
        return new IntersectionNode(left, right);
    }
}

ASTNode* make_complement(ASTNode* expr)
{
    if (auto p = dynamic_cast<ComplementNode*>(expr)) {
        ASTNode* q = p->expr()->clone();
        delete p;
        return q;
    } else {
        return new ComplementNode(expr);
    }
}
