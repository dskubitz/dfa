#include <iostream>
#include <unordered_set>
#include <iomanip>

#include "ASTNode.h"
#include "TransitionTable.h"
#include "TreeFunctions.h"
#include "Parser.h"
#include "PrettyPrinter.h"
#include "Lexer.h"

std::string stoupper(const std::string& str)
{
    std::string res;
    for (auto ch : str) {
        res.push_back(static_cast<char>(toupper(ch)));
    }
    return res;
}

// Hack solution for indenting
class Out {
    std::ostream& ostream_;
    int indent_{0};

public:
    Out(std::ostream& os)
            : ostream_(os) { }

    template<class T>
    friend std::ostream& operator<<(Out& os, const T& arg)
    {
        for (int i = 0; i < os.indent_ << 2; ++i) {
            os.ostream_ << ' ';
        }
        os.ostream_ << arg;
        return os.ostream_;
    }

    void indent() { indent_ += 1; }

    void dedent()
    {
        assert(indent_);
        indent_ -= 1;
    }
};

// Messing around with output
void output_transition_table(const TransitionTable& table)
{
    Out out(std::cout);
    out << "namespace lex {\n";

    std::unordered_set<std::string> set;
    std::vector<std::string> vec;
    out << "enum Token {\n";
    out.indent();
    for (auto& st : table.final_states()) {
        if (set.insert(st.second).second) {
            out << stoupper(st.second) << ",\n";
            vec.push_back(st.second);
        }
    }
    out.dedent();
    out << "};\n\n";

    out << "const char* strings[" << vec.size() << "] = {\n";
    out.indent();
    for (auto& st : vec) {
        out << "\"" << st << "\",\n";
    }
    out.dedent();
    out << "};\n\n";

    out << "Token states[" << table.size() << "] = {\n";
    out.indent();
    for (int i = 0; i < table.size(); ++i) {
        auto it = table.final_states().find(i);
        if (it != table.final_states().end()) {
            out << it->first << ' ' << stoupper(it->second) << ",\n";
        } else {
            out << i << ",\n";
        }
    }
    out.dedent();
    out << "};\n\n";

    out << "int transition_table[" << table.size() << "][128] = {\n";
    out.indent();
    for (int st = 0; st < table.size();) {
        int i = st++;
        out << "{";
        for (int j = 0; j < 128;) {
            std::cout << std::setw(3) << table[i][j++];
            if (j == 128) {
                break;
            }
            std::cout << ", ";
            if (j % 16 == 0) {
                std::cout << "\n";
                out << ' ';
            }
        }
        if (st == table.size())
            std::cout << "},\n";
        else
            std::cout << "},\n\n";
    }
    out.dedent();
    out << "};\n";

    out << "}";// DFA
}

int main()
{
    std::unique_ptr<ASTNode> re = Parser{}.parse(
            {
                    {"[0-9]+[Ee](\\+|\\-)?[0-9]+", "float"},
            }
    );
    PrettyPrinter{std::cout}.print(re.get());
    TreeFunctions functions(re.get());
    TransitionTable table;
    make_transition_table(functions, table);

    std::stringstream ss;
    Lexer lexer(table, ss);
    ss << "123456789E123456789";
//    output_transition_table(table);

    followpos_graphviz(functions);
    /*
    auto regex = Parser{}.parse(
            {
                    {"and",                    "and"},
                    {"class",                  "class"},
                    {"else",                   "else"},
                    {"false",                  "false"},
                    {"for",                    "for"},
                    {"fun",                    "fun"},
                    {"if",                     "if"},
                    {"nil",                    "nil"},
                    {"or",                     "or"},
                    {"print",                  "print"},
                    {"return",                 "return"},
                    {"super",                  "super"},
                    {"this",                   "this"},
                    {"true",                   "true"},
                    {"var",                    "var"},
                    {"while",                  "while"},
                    {"[0-9]+",                 "number"},
                    {"[0-9]+\\.[0-9]+",        "float"},
                    {"\\\"[^\\\"]*\"",         "string"},
                    {"[A-Za-z_][A-Za-z0-9_]*", "identifier"},
                    {"==",                     "equal_equal"},
                    {"!=",                     "bang_equal"},
                    {"<=",                     "less_equal"},
                    {">=",                     "greater_equal"},
                    {"\\<",                    "less"},
                    {"\\>",                    "greater"},
                    {"\\+",                    "plus"},
                    {"\\-",                    "minus"},
                    {"\\*",                    "star"},
                    {"\\/",                    "slash"},
                    {"\\=",                    "equal"},
                    {"\\.",                    "dot"},
                    {"\n",                     "new_line"},
                    {"[ \t\v\f]+",             "space"},
                    {".",                      "error"},
            }
    );
    auto table = make_transition_table(regex.get());
    output_transition_table(table);

    */
    return 0;
}
