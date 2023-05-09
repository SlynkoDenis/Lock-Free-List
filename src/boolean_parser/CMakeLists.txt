set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74.0 REQUIRED)

set(SOURCES
    BooleanCalculator.cpp
    Evaluator.cpp
    Parser.cpp
    )

add_library(boolean_parser STATIC ${SOURCES})

target_sources(boolean_parser PUBLIC
    AST.h
    ASTAdapted.h
    BooleanCalculator.h
    Evaluator.h
    Parser.h
    ParserDef.h
    Utils.h
    )

target_include_directories(boolean_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(boolean_parser PUBLIC
    Boost::boost
    utils
    )
