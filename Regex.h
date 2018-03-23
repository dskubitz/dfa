#ifndef RE_H
#define RE_H

#include <array>
#include <string>
#include <memory>
#include <bitset>
#include <vector>

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
                    120, 121, 122, 123, 124, 125, 126,
};
using Bitset = std::bitset<128>;

inline int first(const Bitset& set)
{
    if (set.none())
        return 0;
    for (int i = 0; i < set.size(); ++i)
        if (set.test(static_cast<unsigned>(i)))
            return i;
}

//@formatter:off
class RegexNode;
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
    virtual void visit(const RegexNode*)=0;
    virtual void visit(const Closure*)=0;
    virtual void visit(const Concat*)=0;
    virtual void visit(const Union*)=0;
    virtual void visit(const Intersection*)=0;
    virtual void visit(const Complement*)=0;
    virtual void visit(const Symbol*)=0;
    virtual void visit(const Epsilon*)=0;
    virtual void visit(const Empty*)=0;
};

class RegexNode {
public:
    virtual ~RegexNode() = default;
    virtual void accept(RegexVisitor* v) const =0;
    virtual RegexNode* clone() const = 0;
    virtual bool equiv(const RegexNode* node) const =0;
    virtual bool comp(const RegexNode* node) const =0;
    virtual size_t hash_code() const =0;
    virtual std::string to_string() const =0;
};

class Closure : public RegexNode {
public:
    explicit Closure(RegexNode* node);
    ~Closure() override;
    void accept(RegexVisitor* v) const override;
    const RegexNode* expr() const;
    bool equiv(const RegexNode* node) const override;
    bool comp(const RegexNode* node) const override;
    size_t hash_code() const override;
    Closure* clone() const override;
    std::string to_string() const override;
private:
    RegexNode* expr_;
};

class Concat : public RegexNode {
public:
    Concat(RegexNode* left, RegexNode* right);
    ~Concat() override;
    void accept(RegexVisitor* v) const override;
    Concat* clone() const override;
    bool equiv(const RegexNode* node) const override;
    bool comp(const RegexNode* node) const override;
    std::string to_string() const override;
    size_t hash_code() const override;
    const RegexNode* left() const;
    const RegexNode* right() const;

private:
    RegexNode* left_;
    RegexNode* right_;
};

class Union : public RegexNode {
public:
    Union(RegexNode* left, RegexNode* right);
    ~Union() override;
    void accept(RegexVisitor* v) const override;
    Union* clone() const override;
    bool comp(const RegexNode* node) const override;
    bool equiv(const RegexNode* node) const override;
    const RegexNode* left() const;
    size_t hash_code() const override;
    const RegexNode* right() const;
    std::string to_string() const override;

private:
    RegexNode* left_;
    RegexNode* right_;
};

class Intersection : public RegexNode {
public:
    Intersection(RegexNode* left, RegexNode* right);
    ~Intersection() override;
    void accept(RegexVisitor* v) const override;
    Intersection* clone() const override;
    bool comp(const RegexNode* node) const override;
    bool equiv(const RegexNode* node) const override;
    size_t hash_code() const override;
    const RegexNode* left() const;
    const RegexNode* right() const;
    std::string to_string() const override;

private:
    RegexNode* left_;
    RegexNode* right_;
};

class Complement : public RegexNode {
public:
    explicit Complement(RegexNode* expr);
    ~Complement() override;
    void accept(RegexVisitor* v) const override;
    Complement* clone() const override;
    bool comp(const RegexNode* node) const override;
    size_t hash_code() const override;
    bool equiv(const RegexNode* node) const override;
    const RegexNode* expr() const;
    std::string to_string() const override;

private:
    RegexNode* expr_;
};

class Symbol : public RegexNode {
public:
    explicit Symbol(char value);
    explicit Symbol(Bitset values);
    void accept(RegexVisitor* v) const override;
    Symbol* clone() const override;
    bool equiv(const RegexNode* node) const override;
    bool comp(const RegexNode* node) const override;
    size_t hash_code() const override;
    const Bitset& values() const;
    std::string to_string() const override;

private:
    Bitset set_;
};

class Epsilon : public RegexNode {
public:
    void accept(RegexVisitor* v) const override;
    Epsilon* clone() const override;
    bool equiv(const RegexNode* node) const override;
    bool comp(const RegexNode* node) const override;
    size_t hash_code() const override;
    std::string to_string() const override;
};

class Empty : public RegexNode {
public:
    void accept(RegexVisitor* v) const override;
    RegexNode* clone() const override;
    bool comp(const RegexNode* node) const override;
    size_t hash_code() const override;
    bool equiv(const RegexNode* node) const override;
    std::string to_string() const override;
};

class Regex {
public:
    Regex() noexcept = default;
    Regex(const Regex& regex);
    explicit Regex(RegexNode* ptr);
    Regex(Regex&& regex) noexcept;
    explicit Regex(std::unique_ptr<RegexNode> ptr);
    Regex& operator=(const Regex& regex);
    Regex& operator=(Regex&& regex) noexcept;

    void swap(Regex& other) noexcept;

    friend bool operator==(const Regex& lhs, const Regex& rhs);
    friend bool operator!=(const Regex& lhs, const Regex& rhs);
    friend bool operator<(const Regex& lhs, const Regex& rhs);
    friend bool operator>(const Regex& lhs, const Regex& rhs);
    friend bool operator<=(const Regex& lhs, const Regex& rhs);
    friend bool operator>=(const Regex& lhs, const Regex& rhs);

    RegexNode* get() noexcept;
    RegexNode* operator->() noexcept;

    const RegexNode* get() const noexcept;
    const RegexNode* operator->() const noexcept;

private:
    std::unique_ptr<RegexNode> ptr_;
};

namespace std {
template<>
struct hash<Regex> {
    size_t operator()(const Regex& regex) const noexcept
    {
        return regex.get()->hash_code();
    }
};
}

inline void swap(Regex& lhs, Regex& rhs) noexcept
{
    lhs.swap(rhs);
}

void print(const std::vector<Regex>& rvec);

RegexNode* make_union(RegexNode* left, RegexNode* right);
RegexNode* make_cat(RegexNode* left, RegexNode* right);
RegexNode* make_star(RegexNode* expr);
RegexNode* make_intersection(RegexNode* left, RegexNode* right);
RegexNode* make_complement(RegexNode* expr);
#endif //RE_H
