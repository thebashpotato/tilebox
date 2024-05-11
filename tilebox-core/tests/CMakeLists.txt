include(GoogleTest)

#
# Add all test source files
#
set(TEST_SOURCE_FILES "utils_tests.cpp" "x11_tests.cpp" "geometry_tests.cpp")

#
# Declare a custom name for the text executable
#
set(PROJECT_UNIT_TEST "${PROJECT_NAME}-tests")

#
# Add all test sources to the executable, and any other sources
#
add_executable(${PROJECT_UNIT_TEST} ${TEST_SOURCE_FILES})

#
# Link all libs to test executable
#
target_include_directories(${PROJECT_UNIT_TEST} PUBLIC ${APP_INCLUDE_DIR})
target_link_libraries(${PROJECT_UNIT_TEST} PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_UNIT_TEST} PRIVATE tilebox_workspace_project_options gtest gtest_main ${X11_LIBRARIES} tilebox-core)

#
# Instruct gtest to discover all tests
#
gtest_discover_tests(${PROJECT_UNIT_TEST})
