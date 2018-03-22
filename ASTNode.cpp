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
bool CharNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool CharNode::equiv(const StarNode& node) const { return false; }
bool CharNode::equiv(const CatNode& node) const { return false; }
bool CharNode::equiv(const UnionNode& node) const { return false; }
bool CharNode::equiv(const EpsilonNode& node) const { return false; }
bool CharNode::equiv(const EmptyNode& node) const { return false; }
bool CharNode::equiv(const IntersectionNode& node) const { return false; }
bool CharNode::equiv(const ComplementNode& node) const { return false; }
//@formatter:on

bool CharNode::equiv(const CharNode& node) const
{
    return node.value_ == value_;
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
bool StarNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool StarNode::equiv(const CatNode& node) const { return false; }
bool StarNode::equiv(const UnionNode& node) const { return false; }
bool StarNode::equiv(const CharNode& node) const { return false; }
bool StarNode::equiv(const IntersectionNode& node) const { return false; }
bool StarNode::equiv(const ComplementNode& node) const { return false; }
bool StarNode::equiv(const EpsilonNode& node) const { return false; }
bool StarNode::equiv(const EmptyNode& node) const { return false; }
//@formatter:on

bool StarNode::equiv(const StarNode& node) const
{
    return expr_->equiv(node.expr_);
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
bool CatNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool CatNode::equiv(const StarNode& node) const { return false; }
bool CatNode::equiv(const UnionNode& node) const { return false; }
bool CatNode::equiv(const CharNode& node) const { return false; }
bool CatNode::equiv(const EpsilonNode& node) const { return false; }
bool CatNode::equiv(const EmptyNode& node) const { return false; }
bool CatNode::equiv(const IntersectionNode& node) const { return false; }
bool CatNode::equiv(const ComplementNode& node) const { return false; }
//@formatter:on

bool CatNode::equiv(const CatNode& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
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
bool UnionNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool UnionNode::equiv(const StarNode& node) const { return false; }
bool UnionNode::equiv(const CatNode& node) const { return false; }
bool UnionNode::equiv(const CharNode& node) const { return false; }
bool UnionNode::equiv(const EpsilonNode& node) const { return false; }
bool UnionNode::equiv(const EmptyNode& node) const { return false; }
bool UnionNode::equiv(const IntersectionNode& node) const { return false; }
bool UnionNode::equiv(const ComplementNode& node) const { return false; }
//@formatter:on

bool UnionNode::equiv(const UnionNode& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
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
bool EpsilonNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool EpsilonNode::equiv(const StarNode& node) const { return false; }
bool EpsilonNode::equiv(const CatNode& node) const { return false; }
bool EpsilonNode::equiv(const UnionNode& node) const { return false; }
bool EpsilonNode::equiv(const CharNode& node) const { return false; }
bool EpsilonNode::equiv(const EmptyNode& node) const { return false; }
bool EpsilonNode::equiv(const IntersectionNode& node) const { return false; }
bool EpsilonNode::equiv(const ComplementNode& node) const { return false; }
//@formatter:on

bool EpsilonNode::equiv(const EpsilonNode& node) const { return true; }

void EmptyNode::accept(ASTVisitor* v) const
{
    v->visit(this);
}

ASTNode* EmptyNode::clone() const
{
    return new EmptyNode;
}

//@formatter:off
bool EmptyNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool EmptyNode::equiv(const StarNode& node) const { return false; }
bool EmptyNode::equiv(const CatNode& node) const { return false; }
bool EmptyNode::equiv(const UnionNode& node) const { return false; }
bool EmptyNode::equiv(const CharNode& node) const { return false; }
bool EmptyNode::equiv(const EpsilonNode& node) const { return false; }
bool EmptyNode::equiv(const IntersectionNode& node) const { return false; }
bool EmptyNode::equiv(const ComplementNode& node) const { return false; }
//@formatter:on

bool EmptyNode::equiv(const EmptyNode& node) const { return true; }

void IntersectionNode::accept(ASTVisitor* v) const { v->visit(this); }

IntersectionNode* IntersectionNode::clone() const
{
    return new IntersectionNode(left_->clone(), right_->clone());
}

//@formatter:off
bool IntersectionNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool IntersectionNode::equiv(const StarNode& node) const { return false; }
bool IntersectionNode::equiv(const CatNode& node) const { return false; }
bool IntersectionNode::equiv(const UnionNode& node) const { return false; }
bool IntersectionNode::equiv(const CharNode& node) const { return false; }
bool IntersectionNode::equiv(const EpsilonNode& node) const { return false; }
bool IntersectionNode::equiv(const ComplementNode& node) const { return false; }
bool IntersectionNode::equiv(const EmptyNode& node) const { return false; }
//@formatter:on

bool IntersectionNode::equiv(const IntersectionNode& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
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
bool ComplementNode::equiv(const ASTNode* node) const { return node->equiv(*this); }
bool ComplementNode::equiv(const StarNode& node) const { return false; }
bool ComplementNode::equiv(const CatNode& node) const { return false; }
bool ComplementNode::equiv(const UnionNode& node) const { return false; }
bool ComplementNode::equiv(const IntersectionNode& node) const { return false; }
bool ComplementNode::equiv(const CharNode& node) const { return false; }
bool ComplementNode::equiv(const EpsilonNode& node) const { return false; }
bool ComplementNode::equiv(const EmptyNode& node) const { return false; }
//@formatter:on

bool ComplementNode::equiv(const ComplementNode& node) const
{
    return expr_->equiv(node.expr_);
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
