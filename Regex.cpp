#include "Regex.h"
#include <sstream>

Closure::Closure(Regex* node)
        :expr_(node)
{
}

void Closure::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const Regex* Closure::expr() const { return expr_; }

Closure::~Closure()
{
    delete expr_;
}

Closure* Closure::clone() const
{
    return new Closure(expr_->clone());
}

//@formatter:off
bool Closure::equiv(const Regex* node) const { return node->equiv(*this); }
bool Closure::equiv(const Concat& node) const { return false; }
bool Closure::equiv(const Union& node) const { return false; }
bool Closure::equiv(const Symbol& node) const { return false; }
bool Closure::equiv(const Intersection& node) const { return false; }
bool Closure::equiv(const Complement& node) const { return false; }
bool Closure::equiv(const Epsilon& node) const { return false; }
bool Closure::equiv(const Empty& node) const { return false; }
//@formatter:on

bool Closure::equiv(const Closure& node) const
{
    return expr_->equiv(node.expr_);
}

std::string Closure::to_string() const
{
    return "(" + expr_->to_string() + "*)";
}

Concat::Concat(Regex* left, Regex* right)
        :left_(left), right_(right)
{
}

void Concat::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const Regex* Concat::left() const { return left_; }

const Regex* Concat::right() const { return right_; }

Concat::~Concat()
{
    delete left_;
    delete right_;
}

Concat* Concat::clone() const
{
    return new Concat(left_->clone(), right_->clone());
}

//@formatter:off
bool Concat::equiv(const Regex* node) const { return node->equiv(*this); }
bool Concat::equiv(const Closure& node) const { return false; }
bool Concat::equiv(const Union& node) const { return false; }
bool Concat::equiv(const Symbol& node) const { return false; }
bool Concat::equiv(const Epsilon& node) const { return false; }
bool Concat::equiv(const Empty& node) const { return false; }
bool Concat::equiv(const Intersection& node) const { return false; }
bool Concat::equiv(const Complement& node) const { return false; }
//@formatter:on

bool Concat::equiv(const Concat& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
}

std::string Concat::to_string() const
{
    return "(" + left_->to_string() + "." + right_->to_string() + ")";
}

Symbol::Symbol(char value)
        :value_(value)
{
}

Union::Union(Regex* left, Regex* right)
        :left_(left), right_(right)
{
}

void Union::accept(RegexVisitor* v) const
{
    v->visit(this);
}

const Regex* Union::left() const { return left_; }

const Regex* Union::right() const { return right_; }

Union::~Union()
{
    delete left_;
    delete right_;
}

Union* Union::clone() const
{
    return new Union(left_->clone(), right_->clone());
}

//@formatter:off
bool Union::equiv(const Regex* node) const { return node->equiv(*this); }
bool Union::equiv(const Closure& node) const { return false; }
bool Union::equiv(const Concat& node) const { return false; }
bool Union::equiv(const Symbol& node) const { return false; }
bool Union::equiv(const Epsilon& node) const { return false; }
bool Union::equiv(const Empty& node) const { return false; }
bool Union::equiv(const Intersection& node) const { return false; }
bool Union::equiv(const Complement& node) const { return false; }
//@formatter:on

bool Union::equiv(const Union& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
}

std::string Union::to_string() const
{
    return "(" + left_->to_string() + "|" + right_->to_string() + ")";
}

void Intersection::accept(RegexVisitor* v) const { v->visit(this); }

Intersection* Intersection::clone() const
{
    return new Intersection(left_->clone(), right_->clone());
}

//@formatter:off
bool Intersection::equiv(const Regex* node) const { return node->equiv(*this); }
bool Intersection::equiv(const Closure& node) const { return false; }
bool Intersection::equiv(const Concat& node) const { return false; }
bool Intersection::equiv(const Union& node) const { return false; }
bool Intersection::equiv(const Symbol& node) const { return false; }
bool Intersection::equiv(const Epsilon& node) const { return false; }
bool Intersection::equiv(const Complement& node) const { return false; }
bool Intersection::equiv(const Empty& node) const { return false; }
//@formatter:on

bool Intersection::equiv(const Intersection& node) const
{
    return left_->equiv(node.left_) && right_->equiv(node.right_);
}

Intersection::~Intersection()
{
    delete left_;
    delete right_;
}

Intersection::Intersection(Regex* left, Regex* right)
        :left_(left), right_(right)
{
}

const Regex* Intersection::left() const { return left_; }

const Regex* Intersection::right() const { return right_; }

std::string Intersection::to_string() const
{
    return "(" + left_->to_string() + "&" + right_->to_string() + ")";
}

void Symbol::accept(RegexVisitor* v) const
{
    v->visit(this);
}

char Symbol::value() const { return value_; }

Symbol* Symbol::clone() const
{
    return new Symbol(value_);
}

//@formatter:off
bool Symbol::equiv(const Regex* node) const { return node->equiv(*this); }
bool Symbol::equiv(const Closure& node) const { return false; }
bool Symbol::equiv(const Concat& node) const { return false; }
bool Symbol::equiv(const Union& node) const { return false; }
bool Symbol::equiv(const Epsilon& node) const { return false; }
bool Symbol::equiv(const Empty& node) const { return false; }
bool Symbol::equiv(const Intersection& node) const { return false; }
bool Symbol::equiv(const Complement& node) const { return false; }
//@formatter:on

bool Symbol::equiv(const Symbol& node) const
{
    return node.value_ == value_;
}

std::string Symbol::to_string() const
{
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}

void Complement::accept(RegexVisitor* v) const { v->visit(this); }

Complement* Complement::clone() const
{
    return new Complement(expr_->clone());
}

//@formatter:off
bool Complement::equiv(const Regex* node) const { return node->equiv(*this); }
bool Complement::equiv(const Closure& node) const { return false; }
bool Complement::equiv(const Concat& node) const { return false; }
bool Complement::equiv(const Union& node) const { return false; }
bool Complement::equiv(const Intersection& node) const { return false; }
bool Complement::equiv(const Symbol& node) const { return false; }
bool Complement::equiv(const Epsilon& node) const { return false; }
bool Complement::equiv(const Empty& node) const { return false; }
//@formatter:on

bool Complement::equiv(const Complement& node) const
{
    return expr_->equiv(node.expr_);
}

Complement::~Complement() { delete expr_; }

Complement::Complement(Regex* expr)
        :expr_(expr) { }

const Regex* Complement::expr() const { return expr_; }

std::string Complement::to_string() const
{
    return "~(" + expr_->to_string() + ")";
}

void Epsilon::accept(RegexVisitor* v) const
{
    v->visit(this);
}

Epsilon* Epsilon::clone() const
{
    return new Epsilon;
}

//@formatter:off
bool Epsilon::equiv(const Regex* node) const { return node->equiv(*this); }
bool Epsilon::equiv(const Closure& node) const { return false; }
bool Epsilon::equiv(const Concat& node) const { return false; }
bool Epsilon::equiv(const Union& node) const { return false; }
bool Epsilon::equiv(const Symbol& node) const { return false; }
bool Epsilon::equiv(const Empty& node) const { return false; }
bool Epsilon::equiv(const Intersection& node) const { return false; }
bool Epsilon::equiv(const Complement& node) const { return false; }
//@formatter:on

bool Epsilon::equiv(const Epsilon& node) const { return true; }

std::string Epsilon::to_string() const
{
    return "\u03b5";
}

void Empty::accept(RegexVisitor* v) const
{
    v->visit(this);
}

Regex* Empty::clone() const
{
    return new Empty;
}

//@formatter:off
bool Empty::equiv(const Regex* node) const { return node->equiv(*this); }
bool Empty::equiv(const Closure& node) const { return false; }
bool Empty::equiv(const Concat& node) const { return false; }
bool Empty::equiv(const Union& node) const { return false; }
bool Empty::equiv(const Symbol& node) const { return false; }
bool Empty::equiv(const Epsilon& node) const { return false; }
bool Empty::equiv(const Intersection& node) const { return false; }
bool Empty::equiv(const Complement& node) const { return false; }
//@formatter:on

bool Empty::equiv(const Empty& node) const { return true; }

std::string Empty::to_string() const
{
    return "\u2205";
}

Regex* make_union(Regex* left, Regex* right)
{
    if (dynamic_cast<Empty*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<Empty*>(right)) {
        delete right;
        return left;
    } else if (*left == *right) {
        delete right;
        return left;
    } else {
        return new Union(left, right);
    }
}

Regex* make_cat(Regex* left, Regex* right)
{
    if (dynamic_cast<Empty*>(left) || dynamic_cast<Empty*>(right)) {
        delete right;
        return left;
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

Regex* make_star(Regex* expr)
{
    if (dynamic_cast<Empty*>(expr)) {
        delete expr;
        return new Epsilon;
    } else if (dynamic_cast<Epsilon*>(expr)) {
        return expr;
    } else if (auto p = dynamic_cast<Closure*>(expr)) {
        Regex* q = p->expr()->clone();
        delete p;
        return make_star(q);
    } else {
        return new Closure(expr);
    }
}

Regex* make_intersection(Regex* left, Regex* right)
{
    if (dynamic_cast<Empty*>(left)) {
        delete right;
        return left;
    } else if (dynamic_cast<Empty*>(right)) {
        delete left;
        return right;
    } else if (*left == *right) {
        delete right;
        return left;
    } else {
        return new Intersection(left, right);
    }
}

Regex* make_complement(Regex* expr)
{
    if (auto p = dynamic_cast<Complement*>(expr)) {
        Regex* q = p->expr()->clone();
        delete p;
        return q;
    } else {
        return new Complement(expr);
    }
}
