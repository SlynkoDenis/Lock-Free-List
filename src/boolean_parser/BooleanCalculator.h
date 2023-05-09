#ifndef LOCK_FREE_LIST_BOOLEAN_CALCULATOR_H_
#define LOCK_FREE_LIST_BOOLEAN_CALCULATOR_H_

#include "AST.h"
#include "Evaluator.h"
#include "macros.h"
#include <string>
#include <unordered_map>


namespace parser {
class BooleanCalculator {
public:
    using ValuesTableType = std::unordered_map<std::string, ValueType>;

    explicit BooleanCalculator(const std::string &expression)
        : formula(expression),
          parsed(parse(expression))
    {}
    explicit BooleanCalculator(std::string &&expression)
        : formula(std::move(expression)),
          parsed(parse(formula))
    {}
    DEFAULT_COPY_SEMANTIC(BooleanCalculator);
    DEFAULT_MOVE_SEMANTIC(BooleanCalculator);
    virtual DEFAULT_DTOR(BooleanCalculator);

    const std::string &GetFormula() const {
        return formula;
    }

    void Optimize() {
        parsed = boost::apply_visitor(ast::ConstantFolder{}, parsed);
    }

    ValueType Evaluate(std::unordered_map<std::string, ValueType> &&table =
                       std::unordered_map<std::string, ValueType>()) {
        return boost::apply_visitor(ast::Evaluator{std::move(table)}, parsed);
    }

private:
    ast::Expression parse(const std::string &expression);

private:
    std::string formula;
    ast::Operand parsed;
};

inline std::ostream &operator<<(std::ostream &os, const BooleanCalculator &calc) {
    os << calc.GetFormula();
    return os;
}
}   // namespace parser

#endif  // LOCK_FREE_LIST_BOOLEAN_CALCULATOR_H_
