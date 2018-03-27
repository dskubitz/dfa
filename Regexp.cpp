#include <Regexp.h>
#include <sstream>
#include <boost/functional/hash.hpp>
#include <experimental/iterator>
#include <iostream>

const std::array<int, 100>
        alphabet {9, 10, 11, 12, 13, 32, 33, 34,
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
                  120, 121, 122, 123, 124, 125, 126,};

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
    if (auto p = dynamic_cast<const Closure*>(node))
        return expr_->equiv(p->expr_);

    return false;
}

std::string Closure::to_string() const
{
    return "(" + expr_->to_string() + "*)";
}

size_t Closure::hash_code() const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, typeid(Closure).hash_code());
    boost::hash_combine(seed, expr_->hash_code());
    return seed;
}

bool Closure::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Closure*>(node))
        return expr_->comp(p->expr_);
    return typeid(Closure).before(typeid(*node));
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
    if (auto p = dynamic_cast<const Concat*>(node))
        return left_->equiv(p->left_) && right_->equiv(p->right_);

    return false;
}

std::string Concat::to_string() const
{
    return "(" + left_->to_string() + "." + right_->to_string() + ")";
}

size_t Concat::hash_code() const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, typeid(Concat).hash_code());
    boost::hash_combine(seed, left_->hash_code());
    boost::hash_combine(seed, right_->hash_code());
    return seed;
}

bool Concat::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Concat*>(node))
        return left_->comp(p->left_);

    return typeid(Concat).before(typeid(*node));
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
    if (auto p = dynamic_cast<const Union*>(node))
        return left_->equiv(p->left_) && right_->equiv(p->right_);

    return false;
}

std::string Union::to_string() const
{
    return "(" + left_->to_string() + "+" + right_->to_string() + ")";
}

size_t Union::hash_code() const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, typeid(Union).hash_code());
    boost::hash_combine(seed, left_->hash_code());
    boost::hash_combine(seed, right_->hash_code());
    return seed;
}

bool Union::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Union*>(node))
        return left_->comp(p->left_);

    return typeid(Union).before(typeid(*node));
}

void Intersection::accept(RegexVisitor* v) const { v->visit(this); }

Intersection* Intersection::clone() const
{
    return new Intersection(left_->clone(), right_->clone());
}

bool Intersection::equiv(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Intersection*>(node))
        return left_->equiv(p->left_) && right_->equiv(p->right_);

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

size_t Intersection::hash_code() const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, typeid(Intersection).hash_code());
    boost::hash_combine(seed, left_->hash_code());
    boost::hash_combine(seed, right_->hash_code());
    return seed;
}

bool Intersection::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Intersection*>(node))
        return left_->comp(p->left_) && right_->comp(p->right_);

    return typeid(Intersection).before(typeid(*node));
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
    if (auto p = dynamic_cast<const Symbol*>(node))
        return set_ == p->set_;

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

size_t Symbol::hash_code() const
{
    return std::hash<Bitset>{}(set_);
}

bool Symbol::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Symbol*>(node))
        return set_.to_string() < p->set_.to_string();

    return typeid(Symbol).before(typeid(*node));
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
    if (auto p = dynamic_cast<const Complement*>(node))
        return expr_->equiv(p->expr_);

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

size_t Complement::hash_code() const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, typeid(Complement).hash_code());
    boost::hash_combine(seed, expr_->hash_code());
    return seed;
}

bool Complement::comp(const RegexNode* node) const
{
    if (auto p = dynamic_cast<const Complement*>(node))
        return expr_->comp(p->expr_);

    return typeid(Complement).before(typeid(*node));
}

void Epsilon::accept(RegexVisitor* v) const
{
    v->visit(this);
}

Epsilon* Epsilon::clone() const
{
    return new Epsilon;
}

bool Epsilon::equiv(const RegexNode* node) const
{
    return bool(dynamic_cast<const Epsilon*>(node));
}

std::string Epsilon::to_string() const
{
    return "\u03b5";
}

size_t Epsilon::hash_code() const
{
    return typeid(Epsilon).hash_code();
}

bool Epsilon::comp(const RegexNode* node) const
{
    return typeid(Epsilon).before(typeid(*node));
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
    return new Empty;
}

//@formatter:off
bool Empty::equiv(const RegexNode* node) const
{
    return bool(dynamic_cast<const Empty*>(node));
}
std::string Empty::to_string() const
{
    return "\u2205";
}

size_t Empty::hash_code() const
{
    return typeid(Empty).hash_code();
}

bool Empty::comp(const RegexNode* node) const
{
    return typeid(Empty).before(typeid(*node));
}

Empty::Empty()
    :RegexNode(Type::Empty)
{
}

RegexNode* make_union(RegexNode* left, RegexNode* right)
{
    if (dynamic_cast<Empty*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<Empty*>(right)) {
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
    if (dynamic_cast<Empty*>(left) || dynamic_cast<Empty*>(right)) {
        delete left;
        delete right;
        return new Empty;
    } else if (dynamic_cast<Epsilon*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<Epsilon*>(right)) {
        delete right;
        return left;
    } else {
        return new Concat(left, right);
    }
}

RegexNode* make_star(RegexNode* expr)
{
    if (dynamic_cast<Empty*>(expr)) {
        delete expr;
        return new Epsilon;
    } else if (dynamic_cast<Epsilon*>(expr)) {
        return expr;
    } else if (auto p = dynamic_cast<Closure*>(expr)) {
        RegexNode* q = p->expr()->clone();
        delete p;
        return make_star(q);
    } else {
        return new Closure(expr);
    }
}

RegexNode* make_intersection(RegexNode* left, RegexNode* right)
{
    if (dynamic_cast<Empty*>(left)) {
        delete right;
        return left;
    } else if (dynamic_cast<Empty*>(right)) {
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
    if (auto p = dynamic_cast<Complement*>(expr)) {
        RegexNode* q = p->expr()->clone();
        delete p;
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
    return lhs.ptr_->comp(rhs.get());
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
    for (unsigned i = 0; i < set.size(); ++i)
        if (set.test(i))
            return i;
    return 0;
}

RegexNode* Regexp::get() noexcept { return ptr_.get(); }

RegexNode* Regexp::operator->() noexcept { return ptr_.get(); }

const RegexNode* Regexp::get() const noexcept { return ptr_.get(); }

const RegexNode* Regexp::operator->() const noexcept { return ptr_.get(); }
