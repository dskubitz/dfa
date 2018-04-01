#include <map>
#include <DFA.h>
#include <Regexp.h>
#include <DerivativeEvaluator.h>
#include <DerivativeClassEvaluator.h>

using State = std::vector<Regexp>;
using StateMap = std::map<State, DFA::Index>;

namespace {
std::tuple<bool, bool, int> accepting(const State& state)
{
    NullableEvaluator N;

    int index = 0;
    int nempty = 0;
    for (const auto& i : state) {
        if (N.evaluate(i)) {
            return {true, false, index};
        }
        if (i->type() == Regex::Type::Empty)
            ++nempty;
        ++index;
    }

    return {false, index == nempty, index};
}
}

DFA make_DFA(const std::vector<Regexp>& regex)
{
    DFA res;
    DFA::TransitionTable& table = res.table;

    StateMap dstates;
    DFA::AcceptMap& accepts = res.accept_map;
    int& dead_state_index = res.dead_state;

    dstates.insert({regex, 0});

    DerivativeEvaluator deriv;
    DerivativeClassEvaluator derivativeClass;

    std::vector<State> unmarked{regex};
    size_t num = 1;
    table.emplace_back();

    while (!unmarked.empty()) {
        State from = unmarked.back();
        unmarked.pop_back();
        std::unordered_set<Bitset> dclass = make_derivative_class(from);

        bool is_accepting;
        bool is_dead;
        int vector_index;
        std::tie(is_accepting, is_dead, vector_index) = accepting(from);
        if (is_accepting)
            accepts.emplace(dstates.at(from), vector_index);
        else if (is_dead)
            dead_state_index = dstates.at(from);

        for (auto& set : dclass) {
            auto c = static_cast<char>(first_occurring(set));
            State to = make_derivative(from, c);

            for (unsigned i = 0; i < set.size(); ++i) {
                if (!set.test(i))
                    continue;

                if (dstates.emplace(to, num).second) {
                    ++num;
                    unmarked.push_back(to);
                    table.emplace_back();
                }
                table[dstates.at(from)][i] = static_cast<int>(dstates.at(to));
            }
        }
    }

    return res;
}
