cmake_minimum_required(VERSION 3.22)
project(LF_LIST)

set(CMAKE_CXX_STANDARD 20)

include(cmake/AddGitSubmodule.cmake)

if(NOT SPOT_LIB_PATH OR NOT SPOT_LIB_PATH MATCHES ".libspot\.a")
    message(FATAL_ERROR "Variable SPOT_LIB_PATH must point to libspot.a")
endif()
if(NOT BUDDY_LIB_PATH OR NOT BUDDY_LIB_PATH MATCHES ".libbddx\.a")
    message(FATAL_ERROR "Variable BUDDY_LIB_PATH must point to libbddx.a")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DNDEBUG)
endif()

add_link_options("-fuse-ld=lld")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -latomic")
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -fsanitize=thread -latomic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_git_submodule(third_party/googletest)
    add_subdirectory(tests)
endif()
