#ifndef DFA_H
#define DFA_H

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

struct DFA {
    using Index = int;
    using TransitionTable = std::vector<std::array<Index, 128> >;
    using AcceptMap = std::unordered_map<Index, Index>;

    TransitionTable table;
    AcceptMap accept_map;
    Index dead_state{-1};
};

class Regexp;
DFA make_DFA(const std::vector<Regexp>& regex);

namespace boost::serialization {
template<class Archive>
void serialize(Archive& archive, DFA& dfa, const unsigned int version)
{
    archive & dfa.table;
    archive & dfa.accept_map;
    archive & dfa.dead_state;
}
} //namespace boost::serialization

#endif //DFA_H
