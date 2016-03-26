cmake_minimum_required(VERSION 3.2)

add_definitions("-std=c++11")
add_definitions("-Wall -Wextra")
add_definitions("-Wno-unknown-pragmas")

include_directories (${CMAKE_SOURCE_DIR})

add_library (seequery
    core/node.cpp
    core/text_node.cpp
    core/html_node.cpp
    core/dom.cpp
    core/collection.cpp
)

add_subdirectory (examples)

set(EXT_PROJECTS_DIR thirdparty)

add_subdirectory(${EXT_PROJECTS_DIR}/catch)
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing

add_subdirectory(tests)
