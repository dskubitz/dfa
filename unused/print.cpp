/*
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
*/
