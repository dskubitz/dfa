#ifndef RE_H
#define RE_H

#include <iosfwd>
#include <memory>
#include <iterator>
#include <array>
#include <string>
#include <iostream>

constexpr static const std::array<int, 100>
        alphabet = {9, 10, 11, 12, 13, 32, 33, 34, 35, 36,
                    37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                    50, 51, 52, 53, 54, 55, 56, 57,
                    58, 59, 60, 61, 62, 63, 64, 65, 66,
                    67, 68, 69, 70, 71, 72, 73, 74,
                    75, 76, 77, 78, 79, 80, 81, 82, 83,
                    84, 85, 86, 87, 88, 89, 90, 91,
                    92, 93, 94, 95, 96, 97, 98, 99, 100,
                    101, 102, 103, 104, 105, 106,
                    107, 108, 109, 110, 111, 112, 113,
                    114, 115, 116, 117, 118, 119,
                    120, 121, 122, 123, 124, 125, 126,
};

//@formatter:off
class ASTNode;
class StarNode;
class CatNode;
class UnionNode;
class IntersectionNode;
class CharNode;
class ComplementNode;
class EpsilonNode;
class EmptyNode;
//class EndmarkerNode;
//@formatter:on

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(const ASTNode*)=0;
    virtual void visit(const StarNode*)=0;
    virtual void visit(const CatNode*)=0;
    virtual void visit(const UnionNode*)=0;
    virtual void visit(const IntersectionNode*)=0;
    virtual void visit(const ComplementNode*)=0;
    virtual void visit(const CharNode*)=0;
    virtual void visit(const EpsilonNode*)=0;
    virtual void visit(const EmptyNode*)=0;
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* v) const =0;
    virtual ASTNode* clone() const = 0;
    // changed signature from bool to int to possibly allow for 3-way compare
    virtual int compare(const ASTNode* node) const =0;
    virtual int compare(const StarNode& node) const =0;
    virtual int compare(const CatNode& node) const =0;
    virtual int compare(const UnionNode& node) const =0;
    virtual int compare(const IntersectionNode& node) const =0;
    virtual int compare(const ComplementNode& node) const =0;
    virtual int compare(const CharNode& node) const =0;
    virtual int compare(const EpsilonNode& node) const =0;
    virtual int compare(const EmptyNode& node) const =0;
};

class StarNode : public ASTNode {
public:
    explicit StarNode(ASTNode* node);
    void accept(ASTVisitor* v) const override;
    const ASTNode* expr() const;
    ~StarNode() override;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    StarNode* clone() const override;
private:
    ASTNode* expr_;
};

class CatNode : public ASTNode {
public:
    CatNode(ASTNode* left, ASTNode* right);
    void accept(ASTVisitor* v) const override;
    const ASTNode* left() const;
    const ASTNode* right() const;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    ~CatNode() override;
    CatNode* clone() const override;

private:
    ASTNode* left_;
    ASTNode* right_;
};

class UnionNode : public ASTNode {
public:
    UnionNode(ASTNode* left, ASTNode* right);
    void accept(ASTVisitor* v) const override;
    const ASTNode* left() const;
    const ASTNode* right() const;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    ~UnionNode() override;
    UnionNode* clone() const override;

private:
    ASTNode* left_;
    ASTNode* right_;
};

class IntersectionNode : public ASTNode {
public:
    IntersectionNode(ASTNode* left, ASTNode* right);
    void accept(ASTVisitor* v) const override;
    IntersectionNode* clone() const override;
    const ASTNode* left() const;
    const ASTNode* right() const;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    ~IntersectionNode() override;
private:
    ASTNode* left_;
    ASTNode* right_;
};

class ComplementNode : public ASTNode {
public:
    void accept(ASTVisitor* v) const override;
    ComplementNode* clone() const override;
    explicit ComplementNode(ASTNode* expr);
    const ASTNode* expr() const;
    int compare(const ASTNode* node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    ~ComplementNode() override;
private:
    ASTNode* expr_;
};

class CharNode : public ASTNode {
public:
    explicit CharNode(char value);
    void accept(ASTVisitor* v) const override;
    char value() const;
    size_t id() const;
    CharNode* clone() const override;
    static size_t max_id();
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
protected:
    static size_t num_;

private:
    size_t id_;
    const char value_;
};

class EpsilonNode : public ASTNode {
public:
    void accept(ASTVisitor* v) const override;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
    EpsilonNode* clone() const override;
};

class EmptyNode : public ASTNode {
public:
    void accept(ASTVisitor* v) const override;
    ASTNode* clone() const override;
    int compare(const ASTNode* node) const override;
    int compare(const StarNode& node) const override;
    int compare(const IntersectionNode& node) const override;
    int compare(const ComplementNode& node) const override;
    int compare(const CatNode& node) const override;
    int compare(const UnionNode& node) const override;
    int compare(const CharNode& node) const override;
    int compare(const EpsilonNode& node) const override;
    int compare(const EmptyNode& node) const override;
};

inline bool operator==(const ASTNode& lhs, const ASTNode& rhs)
{
    return lhs.compare(&rhs) == 0;
}

inline bool operator!=(const ASTNode& lhs, const ASTNode& rhs)
{
    return !(lhs == rhs);
}

ASTNode* make_union(ASTNode* left, ASTNode* right);
ASTNode* make_cat(ASTNode* left, ASTNode* right);
ASTNode* make_star(ASTNode* expr);
ASTNode* make_intersection(ASTNode* left, ASTNode* right);
ASTNode* make_complement(ASTNode* expr);

#endif //RE_H
