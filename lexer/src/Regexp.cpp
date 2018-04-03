#include "lexer/Regexp.h"
#include <sstream>


const std::array<int, 100>
        alphabet{9, 10, 11, 12, 13, 32, 33, 34,
                 35, 36, 37, 38, 39, 40, 41, 42,
                 43, 44, 45, 46, 47, 48, 49, 50,
                 51, 52, 53, 54, 55, 56, 57, 58,
                 59, 60, 61, 62, 63, 64, 65, 66,
                 67, 68, 69, 70, 71, 72, 73, 74,
                 75, 76, 77, 78, 79, 80, 81, 82,
                 83, 84, 85, 86, 87, 88, 89, 90,
                 91, 92, 93, 94, 95, 96, 97, 98,
                 99, 100, 101, 102, 103, 104, 105,
                 106, 107, 108, 109, 110, 111, 112,
                 113, 114, 115, 116, 117, 118, 119,
                 120, 121, 122, 123, 124, 125, 126,
};

Regex::Closure::Closure(Node* node)
        : Node(Type::Closure), expr_(node)
{
}

void Regex::Closure::accept(Visitor* v) const
{
    v->visit(this);
}

const Regex::Node* Regex::Closure::expr() const { return expr_; }

Regex::Closure::~Closure()
{
    delete expr_;
}

Regex::Closure* Regex::Closure::clone() const
{
    return new Closure(expr_->clone());
}

bool Regex::Closure::equiv(const Node* node) const
{
    if (node->type() == Regex::Type::Closure)
        return expr_->equiv(static_cast<const Closure*>(node)->expr_);

    return false;
}

std::string Regex::Closure::to_string() const
{
    return "(" + expr_->to_string() + "*)";
}

bool Regex::Closure::compare(const Node* node) const
{
    if (node->type() == Type::Closure)
        return expr_->compare(static_cast<const Closure*>(node)->expr_);

    return type() < node->type();
}

Regex::Concat::Concat(Node* left, Node* right)
        : Node(Type::Concat), left_(left), right_(right)
{
}

void Regex::Concat::accept(Visitor* v) const
{
    v->visit(this);
}

const Regex::Node* Regex::Concat::left() const { return left_; }

const Regex::Node* Regex::Concat::right() const { return right_; }

Regex::Concat::~Concat()
{
    delete left_;
    delete right_;
}

Regex::Concat* Regex::Concat::clone() const
{
    return new Concat(left_->clone(), right_->clone());
}

bool Regex::Concat::equiv(const Node* node) const
{
    if (node->type() == Type::Concat) {
        auto p = static_cast<const Concat*>(node);
        return left_->equiv(p->left_) && right_->equiv(p->right_);
    }

    return false;
}

std::string Regex::Concat::to_string() const
{
    return "(" + left_->to_string() + "." + right_->to_string() + ")";
}

bool Regex::Concat::compare(const Node* node) const
{
    if (node->type() == Type::Concat)
        return left_->compare(static_cast<const Concat*>(node)->left_);

    return type() < node->type();
}

Regex::Union::Union(Node* left, Node* right)
        : Node(Type::Union), left_(left), right_(right)
{
}

void Regex::Union::accept(Visitor* v) const
{
    v->visit(this);
}

const Regex::Node* Regex::Union::left() const { return left_; }

const Regex::Node* Regex::Union::right() const { return right_; }

Regex::Union::~Union()
{
    delete left_;
    delete right_;
}

Regex::Union* Regex::Union::clone() const
{
    return new Union(left_->clone(), right_->clone());
}

bool Regex::Union::equiv(const Node* node) const
{
    if (node->type() == Type::Union) {
        auto p = static_cast<const Union*>(node);
        return left_->equiv(p->left_) /*&& right_->equiv(p->right_)*/;
    }

    return false;
}

std::string Regex::Union::to_string() const
{
    return "(" + left_->to_string() + "+" + right_->to_string() + ")";
}

bool Regex::Union::compare(const Node* node) const
{
    if (node->type() == Type::Union)
        return left_->compare(static_cast<const Union*>(node)->left_);

    return type() < node->type();
}

void Regex::Intersection::accept(Visitor* v) const { v->visit(this); }

Regex::Intersection* Regex::Intersection::clone() const
{
    return new Intersection(left_->clone(), right_->clone());
}

bool Regex::Intersection::equiv(const Node* node) const
{
    if (node->type() == Type::Intersection) {
        auto p = static_cast<const Intersection*>(node);
        return left_->equiv(p->left_) && right_->equiv(p->right_);
    }

    return false;
}

Regex::Intersection::~Intersection()
{
    delete left_;
    delete right_;
}

Regex::Intersection::Intersection(Node* left, Node* right)
        : Node(Type::Intersection), left_(left), right_(right)
{
}

const Regex::Node* Regex::Intersection::left() const { return left_; }

const Regex::Node* Regex::Intersection::right() const { return right_; }

std::string Regex::Intersection::to_string() const
{
    return "(" + left_->to_string() + "&" + right_->to_string() + ")";
}

bool Regex::Intersection::compare(const Node* node) const
{
    if (node->type() == Type::Intersection) {
        auto p = static_cast<const Intersection*>(node);
        return left_->compare(p->left_) /*&& right_->comp(p->right_)*/;
    }

    return type() < node->type();
}

Regex::Symbol::Symbol(char value)
        : Node(Type::Symbol)
{
    set_.flip(value);
}

Regex::Symbol::Symbol(Bitset values)
        : Node(Type::Symbol), set_(values)
{
}

void Regex::Symbol::accept(Visitor* v) const
{
    v->visit(this);
}

Regex::Symbol* Regex::Symbol::clone() const
{
    return new Symbol(*this);
}

bool Regex::Symbol::equiv(const Node* node) const
{
    if (node->type() == Type::Symbol)
        return set_ == static_cast<const Symbol*>(node)->set_;

    return false;
}

std::string escaped_char(int c)
{
    switch (c) {
    case '\t':
        return "\\t";
    case '\n':
        return "\\n";
    case '\r':
        return "\\r";
    case '\v':
        return "\\v";
    case '\f':
        return "\\f";
    default:
        std::stringstream oss;
        oss << static_cast<char>(c);
        return oss.str();
    }
}

std::string Regex::Symbol::to_string() const
{
    int written = 0;
    bool write_comma = false;
    std::ostringstream oss;
    for (auto it = alphabet.begin(), end = alphabet.end(); it != end;) {
        auto c = static_cast<char>(*it++);

        if (set_.test(c)) {

            if (write_comma) {
                oss << ",";
                write_comma = false;
            }
            oss << escaped_char(c);
            written++;

        } else {
            continue;
        }

        if (it != end) write_comma = true;
    }
    if (written > 1)
        return "{" + oss.str() + "}";
    return oss.str();
}

bool Regex::Symbol::compare(const Node* node) const
{
    if (node->type() == Type::Symbol)
        return set_.to_string()
               < static_cast<const Symbol*>(node)->set_.to_string();

    return type() < node->type();
}

const Bitset& Regex::Symbol::values() const
{
    return set_;
}

void Regex::Complement::accept(Visitor* v) const { v->visit(this); }

Regex::Complement* Regex::Complement::clone() const
{
    return new Complement(expr_->clone());
}

bool Regex::Complement::equiv(const Node* node) const
{
    if (node->type() == Type::Complement)
        return expr_->equiv(static_cast<const Complement*>(node)->expr_);

    return false;
}

Regex::Complement::~Complement() { delete expr_; }

Regex::Complement::Complement(Node* expr)
        : Node(Type::Complement), expr_(expr) { }

const Regex::Node* Regex::Complement::expr() const { return expr_; }

std::string Regex::Complement::to_string() const
{
    return "~(" + expr_->to_string() + ")";
}

bool Regex::Complement::compare(const Node* node) const
{
    if (node->type() == Type::Complement)
        return expr_->compare(static_cast<const Complement*>(node)->expr_);

    return type() < node->type();
}

void Regex::Epsilon::accept(Visitor* v) const
{
    v->visit(this);
}

Regex::Epsilon* Regex::Epsilon::clone() const
{
    return new Epsilon;
}

bool Regex::Epsilon::equiv(const Node* node) const
{
    return node->type() == Type::Epsilon;
}

std::string Regex::Epsilon::to_string() const
{
    return "\u03b5";
}

bool Regex::Epsilon::compare(const Node* node) const
{
    return type() < node->type();
}

Regex::Epsilon::Epsilon()
        : Node(Type::Epsilon)
{
}

void Regex::Empty::accept(Visitor* v) const
{
    v->visit(this);
}

Regex::Node* Regex::Empty::clone() const
{
    return new Empty;
}

bool Regex::Empty::equiv(const Node* node) const
{
    return node->type() == Type::Empty;
}

std::string Regex::Empty::to_string() const
{
    return "\u2205";
}

bool Regex::Empty::compare(const Node* node) const
{
    return type() < node->type();
}

Regex::Empty::Empty()
        : Node(Type::Empty)
{
}

Regex::Node* Regex::make_union(Node* left, Node* right)
{
    if (left->type() == Type::Empty) {
        delete left;
        return right;
    } else if (right->type() == Type::Empty) {
        delete right;
        return left;
    } else if (left->equiv(right)) {
        delete right;
        return left;
    } else {
        return new Union(left, right);
    }
}

Regex::Node* Regex::make_concatenation(Node* left, Node* right)
{
    Type ltype = left->type();
    Type rtype = right->type();
    if (ltype == Type::Empty || rtype == Type::Empty) {
        delete left;
        delete right;
        return new Empty;
    } else if (ltype == Type::Epsilon) {
        delete left;
        return right;
    } else if (rtype == Type::Epsilon) {
        delete right;
        return left;
    } else {
        return new Concat(left, right);
    }
}

Regex::Node* Regex::make_closure(Node* expr)
{
    Type type = expr->type();
    if (type == Type::Empty) {
        delete expr;
        return new Epsilon;
    } else if (type == Type::Epsilon) {
        return expr;
    } else if (type == Type::Closure) {
        Node* q = static_cast<Closure*>(expr)->expr()->clone();
        delete expr;
        return make_closure(q);
    } else {
        return new Closure(expr);
    }
}

Regex::Node* Regex::make_intersection(Node* left, Node* right)
{
    if (left->type() == Type::Empty) {
        delete right;
        return left;
    } else if (right->type() == Type::Empty) {
        delete left;
        return right;
    } else if (left->equiv(right)) {
        delete right;
        return left;
    } else {
        return new Intersection(left, right);
    }
}

Regex::Node* Regex::make_complement(Node* expr)
{
    if (expr->type() == Type::Complement) {
        Node* q = static_cast<Complement*>(expr)->expr()->clone();
        delete expr;
        return q;
    } else {
        return new Complement(expr);
    }
}

void Regexp::swap(Regexp& other) noexcept { ptr_.swap(other.ptr_); }

bool operator==(const Regexp& lhs, const Regexp& rhs)
{
    return lhs.ptr_->equiv(rhs.ptr_.get());
}

bool operator!=(const Regexp& lhs, const Regexp& rhs)
{
    return !(rhs == lhs);
}

bool operator<(const Regexp& lhs, const Regexp& rhs)
{
    return lhs.ptr_->compare(rhs.get());
}

bool operator>(const Regexp& lhs, const Regexp& rhs)
{
    return rhs < lhs;
}

bool operator<=(const Regexp& lhs, const Regexp& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const Regexp& lhs, const Regexp& rhs)
{
    return !(lhs < rhs);
}

int first_occurring(const Bitset& set)
{
    for (size_t i = 0; i < set.size(); ++i)
        if (set.test(i))
            return static_cast<int>(i);
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Regexp& regexp)
{
    return os << regexp->to_string();
}

Regex::Node* Regexp::get() noexcept { return ptr_.get(); }

Regex::Node* Regexp::operator->() noexcept { return ptr_.get(); }

const Regex::Node* Regexp::get() const noexcept { return ptr_.get(); }

const Regex::Node* Regexp::operator->() const noexcept { return ptr_.get(); }
