set(TESTS
    html_node
    collection
)

add_library(catch_main catch_main.cpp)

foreach (TEST_NAME ${TESTS})
    add_executable(${TEST_NAME}.test ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME}.test catch_main seequery)
    add_test(${TEST_NAME} ${TEST_NAME}.test)
endforeach ()