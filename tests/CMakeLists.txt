# Tests

file(GLOB_RECURSE TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME_LOWER}-tests ${TEST_FILES})

target_link_libraries(${PROJECT_NAME_LOWER}-tests ${PROJECT_NAME_LOWER} GTest::gtest)

add_test(NAME ${PROJECT_NAME}Tests COMMAND ${PROJECT_NAME_LOWER}-tests)
