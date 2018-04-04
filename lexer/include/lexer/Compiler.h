#ifndef REDERIV_COMPILER_H
#define REDERIV_COMPILER_H

#include <iosfwd>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <map>

class Regexp;

class Compiler {
public:
    void compile(std::istream& input, std::ostream& output);
private:
    using map_type = std::unordered_map<std::string, std::unordered_set<std::string>>;
    Compiler::map_type read_definitions(std::istream& input);
    std::vector<Regexp> make_regular_vector(const map_type& definitions);

    std::map<int, std::string> definition_id_;
};

#endif //REDERIV_COMPILER_H
