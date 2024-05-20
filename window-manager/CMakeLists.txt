project(
    tilebox-wm
    VERSION 0.1.0
    DESCRIPTION "X11 Tiling Window Manager in C++23"
    LANGUAGES CXX)

#
# Set our source directory variable for use through the build
#
set(APP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(APP_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Libraries
#

#
# Declare all source files
#
set(SOURCE_FILES "${APP_SOURCE_DIR}/version.cpp" "${APP_SOURCE_DIR}/window_manager.cpp" "${APP_SOURCE_DIR}/main.cpp")

#
# If dev-mode is enabled add the tests and examples
#
if(DEVELOPER_MODE)
    #
    # Setup unit tests module
    #
    message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode with Tests and Examples Enabled")
    add_subdirectory(tests)
else()
    message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")
endif()

#
# Output build information
#
message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")

#
# Declare the executable and add all source modules
#
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link required dependencies
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE tilebox_workspace_project_options
            tilebox_workspace_project_warnings
            stdc++fs
            fmt
            tilebox-core
            ${X11_LIBRARIES})

#
# Clang tooling needs to be ran after all libraries/targets are declared.
#
if(DEVELOPER_MODE)
    run_clang_format(${APP_SOURCE_DIR})
    run_clang_format(${APP_TEST_DIR})
else()
    # Create an installable target
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()
