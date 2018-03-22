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
int CharNode::compare(const ASTNode* node) const { return node->compare(*this); }
int CharNode::compare(const StarNode& node) const { return 1; }
int CharNode::compare(const CatNode& node) const { return 1; }
int CharNode::compare(const UnionNode& node) const { return 1; }
int CharNode::compare(const EpsilonNode& node) const { return 1; }
int CharNode::compare(const EmptyNode& node) const { return 1; }
int CharNode::compare(const IntersectionNode& node) const { return 1; }
int CharNode::compare(const ComplementNode& node) const { return 1; }
//@formatter:on

int CharNode::compare(const CharNode& node) const
{
    return node.value_ - value_;
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

//@formatter:off
int StarNode::compare(const ASTNode* node) const { return node->compare(*this); }
int StarNode::compare(const CatNode& node) const { return 1; }
int StarNode::compare(const UnionNode& node) const { return 1; }
int StarNode::compare(const CharNode& node) const { return 1; }
int StarNode::compare(const IntersectionNode& node) const { return 1; }
int StarNode::compare(const ComplementNode& node) const { return 1; }
int StarNode::compare(const EpsilonNode& node) const { return 1; }
int StarNode::compare(const EmptyNode& node) const { return 1; }
//@formatter:on

int StarNode::compare(const StarNode& node) const
{
    return expr_->compare(node.expr_);
    /*
    if (expr_->compare(node.expr_)==0)
        return 0;
    return 1;
    */
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

//@formatter:off
int CatNode::compare(const ASTNode* node) const { return node->compare(*this); }
int CatNode::compare(const StarNode& node) const { return 1; }
int CatNode::compare(const UnionNode& node) const { return 1; }
int CatNode::compare(const CharNode& node) const { return 1; }
int CatNode::compare(const EpsilonNode& node) const { return 1; }
int CatNode::compare(const EmptyNode& node) const { return 1; }
int CatNode::compare(const IntersectionNode& node) const { return 1; }
int CatNode::compare(const ComplementNode& node) const { return 1; }
//@formatter:on

int CatNode::compare(const CatNode& node) const
{
    if ((left_->compare(node.left_) == 0)
        && (right_->compare(node.right_) == 0))
        return 0;
    return 1;
}

UnionNode::UnionNode(ASTNode* left, ASTNode* right)
        :left_(left), right_(right)
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
int UnionNode::compare(const ASTNode* node) const { return node->compare(*this); }
int UnionNode::compare(const StarNode& node) const { return 1; }
int UnionNode::compare(const CatNode& node) const { return 1; }
int UnionNode::compare(const CharNode& node) const { return 1; }
int UnionNode::compare(const EpsilonNode& node) const { return 1; }
int UnionNode::compare(const EmptyNode& node) const { return 1; }
int UnionNode::compare(const IntersectionNode& node) const { return 1; }
int UnionNode::compare(const ComplementNode& node) const { return 1; }
//@formatter:on

int UnionNode::compare(const UnionNode& node) const
{
    if ((left_->compare(node.left_) == 0)
        && (right_->compare(node.right_) == 0))
        return 0;
    return 1;
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
int EpsilonNode::compare(const ASTNode* node) const { return node->compare(*this); }
int EpsilonNode::compare(const StarNode& node) const { return 1; }
int EpsilonNode::compare(const CatNode& node) const { return 1; }
int EpsilonNode::compare(const UnionNode& node) const { return 1; }
int EpsilonNode::compare(const CharNode& node) const { return 1; }
int EpsilonNode::compare(const EmptyNode& node) const { return 1; }
int EpsilonNode::compare(const IntersectionNode& node) const { return 1; }
int EpsilonNode::compare(const ComplementNode& node) const { return 1; }
//@formatter:on

int EpsilonNode::compare(const EpsilonNode& node) const { return 0; }

void EmptyNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

ASTNode* EmptyNode::clone() const
{
    return new EmptyNode;
}

//@formatter:off
int EmptyNode::compare(const ASTNode* node) const { return node->compare(*this); }
int EmptyNode::compare(const StarNode& node) const { return 1; }
int EmptyNode::compare(const CatNode& node) const { return 1; }
int EmptyNode::compare(const UnionNode& node) const { return 1; }
int EmptyNode::compare(const CharNode& node) const { return 1; }
int EmptyNode::compare(const EpsilonNode& node) const { return 1; }
int EmptyNode::compare(const IntersectionNode& node) const { return 1; }
int EmptyNode::compare(const ComplementNode& node) const { return 1; }
//@formatter:on

int EmptyNode::compare(const EmptyNode& node) const { return 0; }

void IntersectionNode::accept(ASTVisitor* v) const { v->visit(this); }

IntersectionNode* IntersectionNode::clone() const
{
    return new IntersectionNode(left_->clone(), right_->clone());
}

//@formatter:off
int IntersectionNode::compare(const ASTNode* node) const { return node->compare(*this); }
int IntersectionNode::compare(const StarNode& node) const { return 1; }
int IntersectionNode::compare(const CatNode& node) const { return 1; }
int IntersectionNode::compare(const UnionNode& node) const { return 1; }
int IntersectionNode::compare(const CharNode& node) const { return 1; }
int IntersectionNode::compare(const EpsilonNode& node) const { return 1; }
int IntersectionNode::compare(const ComplementNode& node) const { return 1; }
int IntersectionNode::compare(const EmptyNode& node) const { return 1; }
//@formatter:on

int IntersectionNode::compare(const IntersectionNode& node) const
{
    if (left_->compare(node.left_) && right_->compare(node.right_))
        return 0;
    return 1;
}

IntersectionNode::~IntersectionNode()
{
    delete left_;
    delete right_;
}

IntersectionNode::IntersectionNode(ASTNode* left, ASTNode* right)
        :left_(left), right_(right)
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
int ComplementNode::compare(const ASTNode* node) const { return node->compare(*this); }
int ComplementNode::compare(const StarNode& node) const { return 1; }
int ComplementNode::compare(const CatNode& node) const { return 1; }
int ComplementNode::compare(const UnionNode& node) const { return 1; }
int ComplementNode::compare(const IntersectionNode& node) const { return 1; }
int ComplementNode::compare(const CharNode& node) const { return 1; }
int ComplementNode::compare(const EpsilonNode& node) const { return 1; }
int ComplementNode::compare(const EmptyNode& node) const { return 1; }
//@formatter:on

int ComplementNode::compare(const ComplementNode& node) const
{
    if (expr_->compare(node.expr_))
        return 0;
    return 1;
}

ComplementNode::~ComplementNode() { delete expr_; }

ComplementNode::ComplementNode(ASTNode* expr)
        :expr_(expr) { }

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
