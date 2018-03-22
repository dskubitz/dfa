#ifndef RE_H
#define RE_H

#include <array>
#include <string>

constexpr static const std::array<int, 100>
        alphabet = {9, 10, 11, 12, 13, 32, 33, 34,
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

//@formatter:off
class Regex;
class Closure;
class Concat;
class Union;
class Intersection;
class Symbol;
class Complement;
class Epsilon;
class Empty;
//@formatter:on

class RegexVisitor {
public:
    virtual ~RegexVisitor() = default;
    virtual void visit(const Regex*)=0;
    virtual void visit(const Closure*)=0;
    virtual void visit(const Concat*)=0;
    virtual void visit(const Union*)=0;
    virtual void visit(const Intersection*)=0;
    virtual void visit(const Complement*)=0;
    virtual void visit(const Symbol*)=0;
    virtual void visit(const Epsilon*)=0;
    virtual void visit(const Empty*)=0;
};

class Regex {
public:
    virtual ~Regex() = default;
    virtual void accept(RegexVisitor* v) const =0;
    virtual Regex* clone() const = 0;
    virtual bool equiv(const Regex* node) const =0;
    virtual bool equiv(const Closure& node) const =0;
    virtual bool equiv(const Concat& node) const =0;
    virtual bool equiv(const Union& node) const =0;
    virtual bool equiv(const Intersection& node) const =0;
    virtual bool equiv(const Complement& node) const =0;
    virtual bool equiv(const Symbol& node) const =0;
    virtual bool equiv(const Epsilon& node) const =0;
    virtual bool equiv(const Empty& node) const =0;
    virtual std::string to_string() const =0;
};

class Closure : public Regex {
public:
    explicit Closure(Regex* node);
    ~Closure() override;
    void accept(RegexVisitor* v) const override;
    const Regex* expr() const;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    Closure* clone() const override;
    std::string to_string() const override;
private:
    Regex* expr_;
};

class Concat : public Regex {
public:
    Concat(Regex* left, Regex* right);
    ~Concat() override;
    void accept(RegexVisitor* v) const override;
    Concat* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    std::string to_string() const override;
    const Regex* left() const;
    const Regex* right() const;

private:
    Regex* left_;
    Regex* right_;
};

class Union : public Regex {
public:
    Union(Regex* left, Regex* right);
    ~Union() override;
    void accept(RegexVisitor* v) const override;
    Union* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    const Regex* left() const;
    const Regex* right() const;
    std::string to_string() const override;

private:
    Regex* left_;
    Regex* right_;
};

class Intersection : public Regex {
public:
    Intersection(Regex* left, Regex* right);
    ~Intersection() override;
    void accept(RegexVisitor* v) const override;
    Intersection* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    const Regex* left() const;
    const Regex* right() const;
    std::string to_string() const override;

private:
    Regex* left_;
    Regex* right_;
};

class Complement : public Regex {
public:
    explicit Complement(Regex* expr);
    ~Complement() override;
    void accept(RegexVisitor* v) const override;
    Complement* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    const Regex* expr() const;
    std::string to_string() const override;

private:
    Regex* expr_;
};

class Symbol : public Regex {
public:
    explicit Symbol(char value);
    void accept(RegexVisitor* v) const override;
    Symbol* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    char value() const;
    std::string to_string() const override;

private:
    size_t id_;
    const char value_;
};

class Epsilon : public Regex {
public:
    void accept(RegexVisitor* v) const override;
    Epsilon* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    std::string to_string() const override;
};

class Empty : public Regex {
public:
    void accept(RegexVisitor* v) const override;
    Regex* clone() const override;
    bool equiv(const Regex* node) const override;
    bool equiv(const Closure& node) const override;
    bool equiv(const Intersection& node) const override;
    bool equiv(const Complement& node) const override;
    bool equiv(const Concat& node) const override;
    bool equiv(const Union& node) const override;
    bool equiv(const Symbol& node) const override;
    bool equiv(const Epsilon& node) const override;
    bool equiv(const Empty& node) const override;
    std::string to_string() const override;
};

inline bool operator==(const Regex& lhs, const Regex& rhs)
{
    return lhs.equiv(&rhs);
}

inline bool operator!=(const Regex& lhs, const Regex& rhs)
{
    return !(lhs == rhs);
}

Regex* make_union(Regex* left, Regex* right);
Regex* make_cat(Regex* left, Regex* right);
Regex* make_star(Regex* expr);
Regex* make_intersection(Regex* left, Regex* right);
Regex* make_complement(Regex* expr);

#endif //RE_H
