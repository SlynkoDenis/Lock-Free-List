#ifndef LOCK_FREE_LIST_STATEFUL_AUTOMATON_H_
#define LOCK_FREE_LIST_STATEFUL_AUTOMATON_H_

#include "macros.h"
#include "BooleanCalculator.h"
#include <spot/twa/bddprint.hh>
#include <spot/parseaut/public.hh>
#include <unordered_map>
#include <unordered_set>


namespace lf {
class StatefulAutomaton {
public:
    using EdgeLabelsMapType = std::unordered_map<int, parser::BooleanCalculator>;
    using ValuesTableType = parser::BooleanCalculator::ValuesTableType;

    explicit StatefulAutomaton(spot::twa_graph_ptr g)
        : graph(g),
          state(g->get_init_state_number()),
          labelsMap(getEdgeLabels(g)),
          valuesTable(getEdgeLabelsNames(g))
    {}
    DEFAULT_COPY_SEMANTIC(StatefulAutomaton);
    DEFAULT_MOVE_SEMANTIC(StatefulAutomaton);
    virtual DEFAULT_DTOR(StatefulAutomaton);

    void operator()(const std::string &varName);

    template <typename... StringT>
    void operator()(StringT... vars) {
        for (auto t : graph->out(state)) {
            auto it = labelsMap.find(t.cond.id());
            ASSERT(it != labelsMap.end());

            if (it->second.Evaluate(std::move(getValuesTable(vars...)))) {
                state = t.dst;
                return;
            }
        }

        std::cerr << "No transition from state " << state << " on event ";
        ((std::cerr << ", " << vars), ...);
        std::cerr << std::endl;
        UNREACHABLE();
    }

private:
    static EdgeLabelsMapType getEdgeLabels(spot::twa_graph_ptr graph);
    static ValuesTableType getEdgeLabelsNames(spot::twa_graph_ptr graph);

    ValuesTableType getValuesTable(const std::string &varName) const;

    template <typename... StringT>
    ValuesTableType getValuesTable(StringT... vars) const {
        ValuesTableType table{valuesTable};
        ((table[vars] = true), ...);
        return table;
    }

private:
    spot::twa_graph_ptr graph;
    spot::twa_graph::state_num state;

    // map from label id to the corresponding boolean calculator
    EdgeLabelsMapType labelsMap;
    ValuesTableType valuesTable;
};

inline spot::twa_graph_ptr ParseAutomaton(const char *filename) {
    auto aut = spot::parse_aut(filename, spot::make_bdd_dict());
    ASSERT(!aut->format_errors(std::cerr) && !aut->aborted);
    return aut->aut;
}
}   // namespace lf

#endif  // LOCK_FREE_LIST_STATEFUL_AUTOMATON_H_
