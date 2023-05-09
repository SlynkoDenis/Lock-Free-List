#ifndef LOCK_FREE_LIST_BOOLEAN_PARSER_H_
#define LOCK_FREE_LIST_BOOLEAN_PARSER_H_

#include "AST.h"
#include <boost/spirit/home/x3.hpp>


// This header file must be included in all cpp files
namespace parser {
namespace impl {
struct ExpressionClass;

using ExpressionType = boost::spirit::x3::rule<ExpressionClass, ast::Expression>;

// Declare top parsing rule
BOOST_SPIRIT_DECLARE(ExpressionType)
}   // namespace impl

impl::ExpressionType grammar();
}   // namespace parser

#endif  // LOCK_FREE_LIST_BOOLEAN_PARSER_H_
