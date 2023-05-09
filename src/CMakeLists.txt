configure_file(automata/insert.hoa ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/insert.hoa COPYONLY)
configure_file(automata/remove.hoa ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/remove.hoa COPYONLY)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74.0 COMPONENTS log log_setup REQUIRED)

add_subdirectory(boolean_parser)
add_subdirectory(utils)

set(SOURCES
    LFList.cpp
    StatefulAutomaton.cpp
    )

add_library(lf_list STATIC ${SOURCES})

target_sources(lf_list PUBLIC
    LFList.h
    StatefulAutomaton.h
    )

target_include_directories(lf_list PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(lf_list PUBLIC
    Boost::log_setup
    Boost::log
    ${SPOT_LIB_PATH}
    ${BUDDY_LIB_PATH}
    boolean_parser
    utils
    )
