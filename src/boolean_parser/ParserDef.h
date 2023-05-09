#ifndef LOCK_FREE_LIST_BOOLEAN_PARSER_DEF_H_
#define LOCK_FREE_LIST_BOOLEAN_PARSER_DEF_H_

#include "AST.h"
#include "ASTAdapted.h"
#include "Parser.h"
#include <iostream>
#include <string>
#include "Utils.h"


// Parser definition
// NB! This file must NOT be included into cpp files (except BooleanParser.cpp)
namespace parser {
namespace x3 = boost::spirit::x3;

namespace impl {
// Lookup

struct Literal : x3::symbols<bool> {
    Literal() {
        add
            ("1", true)
            ("0", false)
            ;
    }
} literal;

struct UnaryOp : x3::symbols<ValueType (*)(ValueType)> {
    UnaryOp() {
        add
            ("!", static_cast<ValueType (*)(ValueType)>(&utils::unaryNot))
            ;
    }
} unaryOp;

struct LogicalOp : x3::symbols<ValueType (*)(ValueType, ValueType)> {
    LogicalOp() {
        add
            ("&", static_cast<ValueType (*)(ValueType, ValueType)>(&utils::logicalAnd))
            ("|", static_cast<ValueType (*)(ValueType, ValueType)>(&utils::logicalOr))
            ;
    }
} logicalOp;

// ADL markers

struct ExpressionClass;
struct LogicalClass;
struct EqualityClass;
struct PrimaryClass;
struct VariableClass;

// Rule declarations

auto const expression = x3::rule<ExpressionClass,   ast::Expression>{"expression"};
auto const logical =    x3::rule<LogicalClass,      ast::Expression>{"logical"};
auto const primary =    x3::rule<PrimaryClass,      ast::Operand>   {"primary"};
auto const variable =   x3::rule<VariableClass,     std::string>    {"variable"};

// Rule defintions, boost naming convention

auto const expression_def =
    logical
    ;

auto const logical_def =
    primary >> *(logicalOp > primary)
    ;

auto const primary_def =
      x3::bool_
    | ('(' > expression > ')')
    | (unaryOp > primary)
    | literal
    | variable
    ;

auto const variable_def =
    x3::raw[x3::lexeme[x3::alpha >> *(x3::alnum | '_')]]
    ;

BOOST_SPIRIT_DEFINE(
    expression,
    logical,
    primary,
    variable
);

struct ExpressionClass {
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator &, Iterator const &last,
                                      Exception const &x, Context const &) {
        std::cout << "Expected " << x.which() << " at \""
                  << std::string{x.where(), last} << "\"" << std::endl;
        return x3::error_handler_result::fail;
    }
};
}   // namespace impl

impl::ExpressionType grammar() {
    return impl::expression;
}
}   // namespace parser

#endif  // LOCK_FREE_LIST_BOOLEAN_PARSER_DEF_H_
