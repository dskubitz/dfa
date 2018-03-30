#include <Regexp.h>
#include <sstream>
#include <boost/functional/hash.hpp>
#include <experimental/iterator>
#include <iostream>

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

Empty Empty::instance;
Epsilon Epsilon::instance;

Closure::Closure(RegexNode* node)
        : RegexNode(Type::Closure), expr_(node)
{
}

void Closure::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const RegexNode* Closure::expr() const { return expr_; }

Closure::~Closure()
{
    delete expr_;
}

Closure* Closure::clone() const
{
    return new Closure(expr_->clone());
}

bool Closure::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Closure)
        return expr_->equiv(static_cast<const Closure*>(node)->expr_);

    return false;
}

std::string Closure::to_string() const
{
    return "(" + expr_->to_string() + "*)";
}

bool Closure::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Closure)
        return expr_->compare(static_cast<const Closure*>(node)->expr_);

    return type() < node->type();
}

Concat::Concat(RegexNode* left, RegexNode* right)
        : RegexNode(Type::Concat), left_(left), right_(right)
{
}

void Concat::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const RegexNode* Concat::left() const { return left_; }

const RegexNode* Concat::right() const { return right_; }

Concat::~Concat()
{
    delete left_;
    delete right_;
}

Concat* Concat::clone() const
{
    return new Concat(left_->clone(), right_->clone());
}

bool Concat::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Concat) {
        auto p = static_cast<const Concat*>(node);
        return left_->equiv(p->left_) && right_->equiv(p->right_);
    }

    return false;
}

std::string Concat::to_string() const
{
    return "(" + left_->to_string() + "." + right_->to_string() + ")";
}

bool Concat::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Concat)
        return left_->compare(static_cast<const Concat*>(node)->left_);

    return type() < node->type();
}

Union::Union(RegexNode* left, RegexNode* right)
        : RegexNode(Type::Union), left_(left), right_(right)
{
}

void Union::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const RegexNode* Union::left() const { return left_; }

const RegexNode* Union::right() const { return right_; }

Union::~Union()
{
    delete left_;
    delete right_;
}

Union* Union::clone() const
{
    return new Union(left_->clone(), right_->clone());
}

bool Union::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Union) {
        auto p = static_cast<const Union*>(node);
        return left_->equiv(p->left_) /*&& right_->equiv(p->right_)*/;
    }

    return false;
}

std::string Union::to_string() const
{
    return "(" + left_->to_string() + "+" + right_->to_string() + ")";
}

bool Union::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Union)
        return left_->compare(static_cast<const Union*>(node)->left_);

    return type() < node->type();
}

void Intersection::accept(RegexVisitor* v) const { v->visit(this); }

Intersection* Intersection::clone() const
{
    return new Intersection(left_->clone(), right_->clone());
}

bool Intersection::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Intersection) {
        auto p = static_cast<const Intersection*>(node);
        return left_->equiv(p->left_) && right_->equiv(p->right_);
    }

    return false;
}

Intersection::~Intersection()
{
    delete left_;
    delete right_;
}

Intersection::Intersection(RegexNode* left, RegexNode* right)
        : RegexNode(Type::Intersection), left_(left), right_(right)
{
}

const RegexNode* Intersection::left() const { return left_; }

const RegexNode* Intersection::right() const { return right_; }

std::string Intersection::to_string() const
{
    return "(" + left_->to_string() + "&" + right_->to_string() + ")";
}

bool Intersection::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Intersection) {
        auto p = static_cast<const Intersection*>(node);
        return left_->compare(p->left_) /*&& right_->comp(p->right_)*/;
    }

    return type() < node->type();
}

Symbol::Symbol(char value)
        : RegexNode(Type::Symbol)
{
    set_.flip(value);
}

Symbol::Symbol(Bitset values)
        : RegexNode(Type::Symbol), set_(values)
{
}

void Symbol::accept(RegexVisitor* v) const
{
    v->visit(this);
}

Symbol* Symbol::clone() const
{
    return new Symbol(*this);
}

bool Symbol::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Symbol)
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

std::string Symbol::to_string() const
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

bool Symbol::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Symbol)
        return set_.to_string()
               < static_cast<const Symbol*>(node)->set_.to_string();

    return type() < node->type();
}

const Bitset& Symbol::values() const
{
    return set_;
}

void Complement::accept(RegexVisitor* v) const { v->visit(this); }

Complement* Complement::clone() const
{
    return new Complement(expr_->clone());
}

bool Complement::equiv(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Complement)
        return expr_->equiv(static_cast<const Complement*>(node)->expr_);

    return false;
}

Complement::~Complement() { delete expr_; }

Complement::Complement(RegexNode* expr)
        : RegexNode(Type::Complement), expr_(expr) { }

const RegexNode* Complement::expr() const { return expr_; }

std::string Complement::to_string() const
{
    return "~(" + expr_->to_string() + ")";
}

bool Complement::compare(const RegexNode* node) const
{
    if (node->type() == RegexNode::Type::Complement)
        return expr_->compare(static_cast<const Complement*>(node)->expr_);

    return type() < node->type();
}

void Epsilon::accept(RegexVisitor* v) const
{
    v->visit(this);
}

Epsilon* Epsilon::clone() const
{
    return &instance;
}

bool Epsilon::equiv(const RegexNode* node) const
{
    return node->type() == RegexNode::Type::Epsilon;
}

std::string Epsilon::to_string() const
{
    return "\u03b5";
}

bool Epsilon::compare(const RegexNode* node) const
{
    return type() < node->type();
}

Epsilon::Epsilon()
        : RegexNode(Type::Epsilon)
{
}

void Empty::accept(RegexVisitor* v) const
{
    v->visit(this);
}

RegexNode* Empty::clone() const
{
    return &instance;
}

bool Empty::equiv(const RegexNode* node) const
{
    return node->type() == RegexNode::Type::Empty;
}

std::string Empty::to_string() const
{
    return "\u2205";
}

bool Empty::compare(const RegexNode* node) const
{
    return type() < node->type();
}

Empty::Empty()
        : RegexNode(Type::Empty)
{
}

RegexNode* make_union(RegexNode* left, RegexNode* right)
{
    if (left->type() == RegexNode::Type::Empty) {
        delete left;
        return right;
    } else if (right->type() == RegexNode::Type::Empty) {
        delete right;
        return left;
    } else if (left->equiv(right)) {
        delete right;
        return left;
    } else {
        return new Union(left, right);
    }
}

RegexNode* make_cat(RegexNode* left, RegexNode* right)
{
    RegexNode::Type ltype = left->type();
    RegexNode::Type rtype = right->type();
    if (ltype == RegexNode::Type::Empty || rtype == RegexNode::Type::Empty) {
        delete left;
        delete right;
        return new Empty;
    } else if (ltype == RegexNode::Type::Epsilon) {
        delete left;
        return right;
    } else if (rtype == RegexNode::Type::Epsilon) {
        delete right;
        return left;
    } else {
        return new Concat(left, right);
    }
}

RegexNode* make_star(RegexNode* expr)
{
    RegexNode::Type type = expr->type();
    if (type == RegexNode::Type::Empty) {
        delete expr;
        return new Epsilon;
    } else if (type == RegexNode::Type::Epsilon) {
        return expr;
    } else if (type == RegexNode::Type::Closure) {
        RegexNode* q = static_cast<Closure*>(expr)->expr()->clone();
        delete expr;
        return make_star(q);
    } else {
        return new Closure(expr);
    }
}

RegexNode* make_intersection(RegexNode* left, RegexNode* right)
{
    if (left->type() == RegexNode::Type::Empty) {
        delete right;
        return left;
    } else if (right->type() == RegexNode::Type::Empty) {
        delete left;
        return right;
    } else if (left->equiv(right)) {
        delete right;
        return left;
    } else {
        return new Intersection(left, right);
    }
}

RegexNode* make_complement(RegexNode* expr)
{
    if (expr->type() == RegexNode::Type::Complement) {
        RegexNode* q = static_cast<Complement*>(expr)->expr()->clone();
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

inline int test(unsigned first, unsigned last, const Bitset& set)
{
    for (; first < last; ++first)
        if (set.test(first))
            return first;
}

static Bitset lowerq{
        "00000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111111111111111111"
};
static Bitset upperq{
        "11111111111111111111111111111111111111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000"
};
static Bitset firstw{
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111111111111111111111"
};
static Bitset firsth{
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111111111"
};
static Bitset thirdh{
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000111111111111111100000000000000000000000000000000"
};
static Bitset thirdw{
        "00000000000000000000000000000000111111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000"
};
static Bitset fifthh{
        "00000000000000000000000000000000000000000000000011111111111111110000000000000000000000000000000000000000000000000000000000000000"
};
static Bitset svnthh{
        "00000000000000001111111111111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
};

int first_occurring(const Bitset& set)
{
    if ((set & lowerq).any()) {
        if ((set & firstw).any()) {
            if ((set & firsth).any())
                return test(0, 16, set);
            else
                return test(16, 32, set);

        } else {
            if ((set & thirdh).any())
                return test(32, 48, set);
            else
                return test(48, 64, set);
        }
    } else if ((set & upperq).any()) {
        if ((set & thirdw).any()) {
            if ((set & fifthh).any())
                return test(64, 80, set);
            else
                return test(80, 96, set);

        } else {
            if ((set & svnthh).any())
                return test(96, 112, set);
            else
                return test(112, 128, set);
        }
    }
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Regexp& regexp)
{
    return os << regexp->to_string();
}

RegexNode* Regexp::get() noexcept { return ptr_.get(); }

RegexNode* Regexp::operator->() noexcept { return ptr_.get(); }

const RegexNode* Regexp::get() const noexcept { return ptr_.get(); }

const RegexNode* Regexp::operator->() const noexcept { return ptr_.get(); }
