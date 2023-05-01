set(SOURCES
    LFList.cpp
    )

add_library(lf_list STATIC ${SOURCES})

target_sources(lf_list PUBLIC
    debug.h
    LFList.h
    macros.h
    )

target_include_directories(lf_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
