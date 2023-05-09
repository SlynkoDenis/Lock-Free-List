#include "ParserDef.h"


namespace parser::impl {
namespace x3 = boost::spirit::x3;

using IteratorType = std::string::const_iterator;
using ContextType = x3::phrase_parse_context<x3::ascii::space_type>::type;

BOOST_SPIRIT_INSTANTIATE(ExpressionType, IteratorType, ContextType)
} // namespace parser::impl
