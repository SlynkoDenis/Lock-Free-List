#ifndef LOCK_FREE_LIST_AST_H_
#define LOCK_FREE_LIST_AST_H_

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3.hpp>
#include <list>
#include <string>


namespace parser {
using ValueType = bool;

namespace ast {
namespace x3 = boost::spirit::x3;

struct Nil {};
struct UnaryOp;
struct BinaryOp;
struct Expression;

struct Operand : x3::variant<
        Nil,
        ValueType,
        std::string,
        x3::forward_ast<UnaryOp>,
        x3::forward_ast<BinaryOp>,
        x3::forward_ast<Expression>> {
    using base_type::base_type;
    using base_type::operator=;
};

struct UnaryOp {
    ValueType (*op)(ValueType);
    Operand rhs;
};

struct BinaryOp {
    ValueType (*op)(ValueType, ValueType);
    Operand lhs;
    Operand rhs;
};

struct Operation {
    ValueType (*op)(ValueType, ValueType);
    Operand rhs;
};

struct Expression {
    Operand lhs;
    std::list<Operation> rhs;
};
}   // namespace ast
}   // namespace parser

#endif  // LOCK_FREE_LIST_AST_H_
