#include "AST.h"
#include "Evaluator.h"
#include "macros.h"
#include "Utils.h"


namespace parser::ast {
// Optimizer

ConstantFolder::result_type ConstantFolder::operator()(Nil) const {
    return result_type{0};
}

ConstantFolder::result_type ConstantFolder::operator()(ValueType n) const {
    return result_type{n};
}

ConstantFolder::result_type ConstantFolder::operator()(const std::string &c) const {
    return result_type{c};
}

ConstantFolder::result_type ConstantFolder::operator()(const Operation &x, const Operand &lhs) const {
    auto rhs = boost::apply_visitor(*this, x.rhs);

    if (utils::holdsAlternative<ValueType>(lhs) && utils::holdsAlternative<ValueType>(rhs)) {
        return result_type{
            x.op(boost::get<ValueType>(lhs), boost::get<ValueType>(rhs))};
    }
    return result_type{BinaryOp{x.op, lhs, rhs}};
}

ConstantFolder::result_type ConstantFolder::operator()(const UnaryOp &x) const {
    auto rhs = boost::apply_visitor(*this, x.rhs);

    /// If the Operand is known, we can directly evaluate the function.
    if (utils::holdsAlternative<ValueType>(rhs)) {
        return result_type{x.op(boost::get<ValueType>(rhs))};
    }
    return result_type{UnaryOp{x.op, rhs}};
}

ConstantFolder::result_type ConstantFolder::operator()(const BinaryOp &x) const {
    auto lhs = boost::apply_visitor(*this, x.lhs);
    auto rhs = boost::apply_visitor(*this, x.rhs);

    /// If both operands are known, we can directly evaluate the function,
    /// else we just update the children with the new expressions.
    if (utils::holdsAlternative<ValueType>(lhs) && utils::holdsAlternative<ValueType>(rhs)) {
        return result_type{
            x.op(boost::get<ValueType>(lhs), boost::get<ValueType>(rhs))};
    }
    return result_type{BinaryOp{x.op, lhs, rhs}};
}

ConstantFolder::result_type ConstantFolder::operator()(const Expression &x) const {
    auto state = boost::apply_visitor(*this, x.lhs);
    for (auto const &oper : x.rhs) {
        state = (*this)(oper, state);
    }
    return result_type{state};
}

// Evaluator

ValueType Evaluator::operator()(Nil) const {
    BOOST_ASSERT(0);
    return 0;
}

ValueType Evaluator::operator()(ValueType n) const {
    return n;
}

ValueType Evaluator::operator()(const std::string &c) const {
    auto it = st.find(c);
    ASSERT(it != st.end());
    return it->second;
}

ValueType Evaluator::operator()(const Operation &x, ValueType lhs) const {
    ValueType rhs = boost::apply_visitor(*this, x.rhs);
    return x.op(lhs, rhs);
}

ValueType Evaluator::operator()(const UnaryOp &x) const {
    ValueType rhs = boost::apply_visitor(*this, x.rhs);
    return x.op(rhs);
}

ValueType Evaluator::operator()(const BinaryOp &x) const {
    ValueType lhs = boost::apply_visitor(*this, x.lhs);
    ValueType rhs = boost::apply_visitor(*this, x.rhs);
    return x.op(lhs, rhs);
}

ValueType Evaluator::operator()(const Expression &x) const {
    ValueType state = boost::apply_visitor(*this, x.lhs);
    for (auto const &oper : x.rhs) {
        state = (*this)(oper, state);
    }
    return state;
}
} // namespace parser::ast
