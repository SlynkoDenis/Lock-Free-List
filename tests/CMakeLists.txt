set(BINARY tests)

set(SOURCES
    LFListTest.cpp
    main.cpp
    )

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC gtest lf_list)
