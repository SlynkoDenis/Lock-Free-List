#include "BooleanCalculator.h"
#include "StatefulAutomaton.h"


namespace lf {
void StatefulAutomaton::operator()(const std::string &varName) {
    for (auto t : graph->out(state)) {
        auto it = labelsMap.find(t.cond.id());
        ASSERT(it != labelsMap.end());

        if (it->second.Evaluate(std::move(getValuesTable(varName)))) {
            state = t.dst;
            return;
        }
    }
    std::cerr << "No transition from state " << state << " on event " << varName << std::endl;
    UNREACHABLE();
}

/* static */
StatefulAutomaton::EdgeLabelsMapType StatefulAutomaton::getEdgeLabels(spot::twa_graph_ptr graph) {
    ASSERT(graph);
    EdgeLabelsMapType labels;

    auto graphDict = graph->get_dict();
    auto numStates = graph->num_states();
    for (decltype(numStates) s = 0; s < numStates; ++s) {
        for (auto t : graph->out(s)) {
            std::stringstream oss;
            spot::bdd_print_formula(oss, graphDict, t.cond);
            [[maybe_unused]] auto res = labels.emplace(t.cond.id(), std::move(oss.str()));
            ASSERT(res.second);
        }
    }
    return labels;
}

/* static */
StatefulAutomaton::ValuesTableType StatefulAutomaton::getEdgeLabelsNames(spot::twa_graph_ptr graph) {
    ASSERT(graph);
    const auto &propositions = graph->ap();
    ValuesTableType names(propositions.size());
    for (auto l : propositions) {
        [[maybe_unused]] auto res = names.emplace(l.ap_name(), false);
        ASSERT(res.second);
    }
    return names;
}

StatefulAutomaton::ValuesTableType StatefulAutomaton::getValuesTable(const std::string &varName) const {
    ValuesTableType table{valuesTable};
    table[varName] = true;
    return table;
}
}   // namespace lf
