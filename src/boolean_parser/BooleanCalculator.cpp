#include "BooleanCalculator.h"
#include "Parser.h"
#include <iostream>


namespace parser {
ast::Expression BooleanCalculator::parse(const std::string &expr) {
    ast::Expression ast{};

    auto first = expr.begin();
    auto last = expr.end();

    auto r = phrase_parse(first, last, grammar(), boost::spirit::x3::ascii::space, ast);

    if (!r || first != last) {
        std::cerr << "Parsing failed at " << std::string(first, last) << std::endl;
        UNREACHABLE();
    }

    return ast;
}
}   // namespace parser
