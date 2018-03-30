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
    virtual ~RegexNode() = default;
    virtual void accept(RegexVisitor* v) const =0;
    virtual RegexNode* clone() const = 0;
    virtual bool equiv(const RegexNode* node) const =0;
    virtual bool compare(const RegexNode* node) const =0;
    virtual std::string to_string() const =0;

    Type type() const noexcept { return type_; }

protected:
    RegexNode(Type type)
            : type_(type) { }

private:
    const Type type_;
};

class Closure : public RegexNode {
public:
    explicit Closure(RegexNode* node);
    Closure(nullptr_t) = delete;
    ~Closure() override;
    void accept(RegexVisitor* v) const override;
    const RegexNode* expr() const;
    bool equiv(const RegexNode* node) const override;
    bool compare(const RegexNode* node) const override;
    Closure* clone() const override;
    std::string to_string() const override;
private:
    RegexNode* expr_;
};

class Concat : public RegexNode {
public:
    Concat(RegexNode* left, RegexNode* right);
    template<typename T>
    Concat(T* left, nullptr_t) = delete;
    template<typename T>
    Concat(nullptr_t, T* right) = delete;
    ~Concat() override;
    void accept(RegexVisitor* v) const override;
    Concat* clone() const override;
    bool equiv(const RegexNode* node) const override;
    bool compare(const RegexNode* node) const override;
    std::string to_string() const override;
    const RegexNode* left() const;
    const RegexNode* right() const;

private:
    RegexNode* left_;
    RegexNode* right_;
};

class Union : public RegexNode {
public:
    Union(RegexNode* left, RegexNode* right);
    template<typename T>
    Union(T* left, nullptr_t) = delete;
    template<typename T>
    Union(nullptr_t, T* right) = delete;
    ~Union() override;
    void accept(RegexVisitor* v) const override;
    Union* clone() const override;
    bool compare(const RegexNode* node) const override;
    bool equiv(const RegexNode* node) const override;
    const RegexNode* left() const;
    const RegexNode* right() const;
    std::string to_string() const override;

private:
    RegexNode* left_;
    RegexNode* right_;
};

class Intersection : public RegexNode {
public:
    Intersection(RegexNode* left, RegexNode* right);
    template<typename T>
    Intersection(T* left, nullptr_t) = delete;
    template<typename T>
    Intersection(nullptr_t, T* right) = delete;
    ~Intersection() override;
    void accept(RegexVisitor* v) const override;
    Intersection* clone() const override;
    bool compare(const RegexNode* node) const override;
    bool equiv(const RegexNode* node) const override;
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
    Complement(nullptr_t) = delete;
    ~Complement() override;
    void accept(RegexVisitor* v) const override;
    Complement* clone() const override;
    bool compare(const RegexNode* node) const override;
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
    bool compare(const RegexNode* node) const override;
    const Bitset& values() const;
    std::string to_string() const override;

private:
    Bitset set_;
};

class Epsilon : public RegexNode {
    static Epsilon instance;
public:
    Epsilon();
    void accept(RegexVisitor* v) const override;
    Epsilon* clone() const override;
    bool equiv(const RegexNode* node) const override;
    bool compare(const RegexNode* node) const override;
    std::string to_string() const override;

    static void* operator new(std::size_t sz) { return &instance; }

    static void operator delete(void* p) { }
};

class Empty : public RegexNode {
    static Empty instance;
public:
    Empty();
    void accept(RegexVisitor* v) const override;
    RegexNode* clone() const override;
    bool compare(const RegexNode* node) const override;
    bool equiv(const RegexNode* node) const override;
    std::string to_string() const override;

    static void* operator new(std::size_t sz) { return &instance; }

    static void operator delete(void* p) { }
};

RegexNode* make_union(RegexNode* left, RegexNode* right);
RegexNode* make_cat(RegexNode* left, RegexNode* right);
RegexNode* make_star(RegexNode* expr);
RegexNode* make_intersection(RegexNode* left, RegexNode* right);
RegexNode* make_complement(RegexNode* expr);

class Regexp {
public:
    explicit Regexp(RegexNode* ptr)
            : ptr_(ptr) { }

    void swap(Regexp& other) noexcept;

    friend bool operator==(const Regexp& lhs, const Regexp& rhs);
    friend bool operator!=(const Regexp& lhs, const Regexp& rhs);
    friend bool operator<(const Regexp& lhs, const Regexp& rhs);
    friend bool operator>(const Regexp& lhs, const Regexp& rhs);
    friend bool operator<=(const Regexp& lhs, const Regexp& rhs);
    friend bool operator>=(const Regexp& lhs, const Regexp& rhs);

    RegexNode* get() noexcept;
    RegexNode* operator->() noexcept;

    const RegexNode* get() const noexcept;
    const RegexNode* operator->() const noexcept;

private:
    std::shared_ptr<RegexNode> ptr_;
};

std::ostream& operator<<(std::ostream& os, const Regexp& regexp);

inline void swap(Regexp& lhs, Regexp& rhs) noexcept
{
    lhs.swap(rhs);
}

#endif //RE_H
