#ifndef LOCK_FREE_LIST_EVALUATOR_H_
#define LOCK_FREE_LIST_EVALUATOR_H_

#include "AST.h"
#include <string>
#include <unordered_map>


namespace parser::ast {
// boost-compatible visitors

struct ConstantFolder {
    using result_type = Operand;

    result_type operator()(Nil) const;

    result_type operator()(ValueType n) const;

    result_type operator()(const std::string &c) const;

    result_type operator()(const Operation &x, Operand const &lhs) const;

    result_type operator()(const UnaryOp &x) const;

    result_type operator()(const BinaryOp &x) const;

    result_type operator()(const Expression &x) const;
};

struct Evaluator {
    using result_type = ValueType;

    explicit Evaluator(std::unordered_map<std::string, ValueType> &&sym) : st(std::move(sym)) {}

    result_type operator()(Nil) const;

    result_type operator()(ValueType n) const;

    result_type operator()(const std::string &c) const;

    result_type operator()(const Operation &x, ValueType lhs) const;

    result_type operator()(const UnaryOp &x) const;

    result_type operator()(const BinaryOp &x) const;

    result_type operator()(const Expression &x) const;

private:
    std::unordered_map<std::string, ValueType> st;
};
} // namespace parser::ast

#endif  // LOCK_FREE_LIST_EVALUATOR_H_
