#include "lexer/Compiler.h"
#include <istream>
#include <sstream>
#include <numeric>
#include <lexer/Regexp.h>
#include <lexer/Parser.h>
#include <lexer/DFA.h>

namespace {
std::string get_name(std::istream& input)
{
    std::string name;
    input >> name;
    return name;
}

void skip_whitespace(std::istream& input)
{
    while (input.good()) {
        int peek = input.peek();
        if (!isspace(peek))
            break;
        input.get();
    }
}

std::string get_regex(std::istream& input)
{
    std::string regex;
    int ch;
    while (input.good()) {
        ch = input.get();
        if (ch == '\n' || ch == EOF) {
            break;
        } else if (ch == '\\') {
            ch = input.get();
            switch (ch) {
            case 'a':
                regex.push_back('\a');
                break;
            case 'b':
                regex.push_back('\b');
                break;
            case 'f':
                regex.push_back('\f');
                break;
            case 'n':
                regex.push_back('\n');
                break;
            case 'r':
                regex.push_back('\r');
                break;
            case 't':
                regex.push_back('\t');
                break;
            default:
                regex.push_back(ch);
                break;
            }
        } else {
            regex.push_back(ch);
        }
    }
    return regex;
}
}

void Compiler::compile(std::istream& input, std::ostream& output)
{
    map_type definitions = read_definitions(input);
    std::vector<Regexp> regular_vector = make_regular_vector(definitions);
    DFA dfa = make_DFA(regular_vector);

    output << "int table[" << dfa.table.size() << "][128] = {\n";
    for (auto& state : dfa.table) {
        int n = 0;
        output << "\t{";
        for (auto& transition : state) {
            output << transition << ", ";
            n++;
            if (n % 16 == 0) {
                output << "\n\t";
            }
        }
        output << "},\n";
    }
    output << "};\n";
    output << "enum {\n";
    output << "\tSTART = 0,\n\t";
    for (auto& i : dfa.accept_map) {
        output << definition_id_.at(i.second) << " = " << i.first << ",\n\t";
    }
    output << "DEAD = " << dfa.dead_state << ",\n";
    output << "};\n";
}

Compiler::map_type Compiler::read_definitions(std::istream& input)
{
    map_type definitions;

    for (std::string line; std::getline(input, line);) {
        std::stringstream linestream(line);

        std::string name = get_name(linestream);
        skip_whitespace(linestream);
        std::string regex = get_regex(linestream);

        definitions[name].insert(regex);
    }

    return definitions;
}

std::vector<Regexp> Compiler::make_regular_vector(const map_type& definitions)
{
    Parser parser;
    int num = 0;
    std::vector<Regexp> rvec;
    definition_id_.clear();

    for (auto& def : definitions) {
        definition_id_[num] = def.first;

        rvec.emplace_back(std::accumulate(
                def.second.begin(), def.second.end(), Regexp(new Regex::Empty),
                [&parser](const auto& lhs, const auto& rhs) {
                    return unify(lhs, parser.parse(rhs));
                }));

        num++;
    }
    return rvec;
}
