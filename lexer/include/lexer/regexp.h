#ifndef RE_H
#define RE_H

#include <array>
#include <string>
#include <memory>
#include <bitset>
#include <vector>

extern const std::array<int, 100> alphabet;

using Bitset = std::bitset<128>;

int first_occurring(const Bitset& set);

namespace Regex {
//@formatter:off
class Node;
class Closure;
class Concat;
class Union;
class Intersection;
class Symbol;
class Complement;
class Epsilon;
class Empty;
//@formatter:on

enum class Type : uint8_t {
    Closure,
    Concat,
    Union,
    Intersection,
    Symbol,
    Complement,
    Epsilon,
    Empty,
};

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const Node*)=0;
    virtual void visit(const Closure*)=0;
    virtual void visit(const Concat*)=0;
    virtual void visit(const Union*)=0;
    virtual void visit(const Intersection*)=0;
    virtual void visit(const Complement*)=0;
    virtual void visit(const Symbol*)=0;
    virtual void visit(const Epsilon*)=0;
    virtual void visit(const Empty*)=0;
};

class Node {
public:
    virtual ~Node() = default;
    virtual void accept(Visitor* v) const =0;
    virtual Node* clone() const = 0;
    virtual bool equiv(const Node* node) const =0;
    virtual bool compare(const Node* node) const =0;
    virtual std::string to_string() const =0;

    Type type() const noexcept { return type_; }

protected:
    Node(Type type)
            : type_(type) { }

private:
    const Type type_;
};

class Closure : public Node {
public:
    explicit Closure(Node* node);
    Closure(nullptr_t) = delete;
    ~Closure() override;
    void accept(Visitor* v) const override;
    const Node* expr() const;
    bool equiv(const Node* node) const override;
    bool compare(const Node* node) const override;
    Closure* clone() const override;
    std::string to_string() const override;
private:
    Node* expr_;
};

class Concat : public Node {
public:
    Concat(Node* left, Node* right);
    template<typename T>
    Concat(T* left, nullptr_t) = delete;
    template<typename T>
    Concat(nullptr_t, T* right) = delete;
    ~Concat() override;
    void accept(Visitor* v) const override;
    Concat* clone() const override;
    bool equiv(const Node* node) const override;
    bool compare(const Node* node) const override;
    std::string to_string() const override;
    const Node* left() const;
    const Node* right() const;

private:
    Node* left_;
    Node* right_;
};

class Union : public Node {
public:
    Union(Node* left, Node* right);
    template<typename T>
    Union(T* left, nullptr_t) = delete;
    template<typename T>
    Union(nullptr_t, T* right) = delete;
    ~Union() override;
    void accept(Visitor* v) const override;
    Union* clone() const override;
    bool compare(const Node* node) const override;
    bool equiv(const Node* node) const override;
    const Node* left() const;
    const Node* right() const;
    std::string to_string() const override;

private:
    Node* left_;
    Node* right_;
};

class Intersection : public Node {
public:
    Intersection(Node* left, Node* right);
    template<typename T>
    Intersection(T* left, nullptr_t) = delete;
    template<typename T>
    Intersection(nullptr_t, T* right) = delete;
    ~Intersection() override;
    void accept(Visitor* v) const override;
    Intersection* clone() const override;
    bool compare(const Node* node) const override;
    bool equiv(const Node* node) const override;
    const Node* left() const;
    const Node* right() const;
    std::string to_string() const override;

private:
    Node* left_;
    Node* right_;
};

class Complement : public Node {
public:
    explicit Complement(Node* expr);
    Complement(nullptr_t) = delete;
    ~Complement() override;
    void accept(Visitor* v) const override;
    Complement* clone() const override;
    bool compare(const Node* node) const override;
    bool equiv(const Node* node) const override;
    const Node* expr() const;
    std::string to_string() const override;

private:
    Node* expr_;
};

class Symbol : public Node {
public:
    explicit Symbol(char value);
    explicit Symbol(Bitset values);
    void accept(Visitor* v) const override;
    Symbol* clone() const override;
    bool equiv(const Node* node) const override;
    bool compare(const Node* node) const override;
    const Bitset& values() const;
    std::string to_string() const override;

private:
    Bitset set_;
};

class Epsilon : public Node {
public:
    Epsilon();
    void accept(Visitor* v) const override;
    Epsilon* clone() const override;
    bool equiv(const Node* node) const override;
    bool compare(const Node* node) const override;
    std::string to_string() const override;
};

class Empty : public Node {
public:
    Empty();
    void accept(Visitor* v) const override;
    Node* clone() const override;
    bool compare(const Node* node) const override;
    bool equiv(const Node* node) const override;
    std::string to_string() const override;
};
}// namespace Regex

Regex::Node* make_union(Regex::Node* left, Regex::Node* right);
Regex::Node* make_concatenation(Regex::Node* left, Regex::Node* right);
Regex::Node* make_closure(Regex::Node* expr);
Regex::Node* make_intersection(Regex::Node* left, Regex::Node* right);
Regex::Node* make_complement(Regex::Node* expr);

class regexp {
public:
    explicit regexp(Regex::Node* ptr)
            : ptr_(ptr) { }

    void swap(regexp& other) noexcept;

    friend bool operator==(const regexp& lhs, const regexp& rhs);
    friend bool operator!=(const regexp& lhs, const regexp& rhs);
    friend bool operator<(const regexp& lhs, const regexp& rhs);
    friend bool operator>(const regexp& lhs, const regexp& rhs);
    friend bool operator<=(const regexp& lhs, const regexp& rhs);
    friend bool operator>=(const regexp& lhs, const regexp& rhs);

    Regex::Node* get() noexcept;
    Regex::Node* operator->() noexcept;

    const Regex::Node* get() const noexcept;
    const Regex::Node* operator->() const noexcept;

private:
    std::shared_ptr<Regex::Node> ptr_;
};

std::ostream& operator<<(std::ostream& os, const regexp& regexp);

inline void swap(regexp& lhs, regexp& rhs) noexcept
{
    lhs.swap(rhs);
}

#endif //RE_H
