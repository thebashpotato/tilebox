project(
    tilebox-core
    VERSION 0.1.0
    DESCRIPTION "XLib wrapper library and common tilebox functionality"
    LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(APP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(APP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(APP_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(APP_EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

#
# Declare all source files
#
set(SOURCE_FILES
    "${APP_SOURCE_DIR}/geometry.cpp"
    "${APP_SOURCE_DIR}/x11/display.cpp"
    "${APP_SOURCE_DIR}/x11/window.cpp"
    "${APP_SOURCE_DIR}/x11/events.cpp"
    "${APP_SOURCE_DIR}/x11/event_loop.cpp"
    "${APP_SOURCE_DIR}/draw/font.cpp"
    "${APP_SOURCE_DIR}/draw/utf8_codec.cpp"
    "${APP_SOURCE_DIR}/draw/draw.cpp")

#
# If dev-mode is enabled add the tests directory
#
if(DEVELOPER_MODE)
    #
    # Setup unit tests module
    #
    message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode with Tests and Examples Enabled")
    add_subdirectory(tests)
    add_subdirectory(examples)
else()
    message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")
endif()

#
# Create the library
#
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})

# Link required dependencies
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE tilebox_workspace_project_options
            tilebox_workspace_project_warnings
            stdc++fs
            fmt
            ${X11_Xft_LIB}
            ${X11_X11_LIB}
            ${Fontconfig_LIBRARY})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${APP_INCLUDE_DIR}>
           $<INSTALL_INTERFACE:include>
           ${X11_INCLUDE_DIR}
           ${FREETYPE_INCLUDE_DIR_ft2build}
           ${FREETYPE_INCLUDE_DIR_freetype2}
           ${Fontconfig_INCLUDE_DIR})

#
# Clang tooling needs to be ran after all libraries/targets are declared
#
if(DEVELOPER_MODE)
    run_clang_format(${APP_SOURCE_DIR})
    run_clang_format(${APP_INCLUDE_DIR})
    run_clang_format(${APP_TEST_DIR})
    run_clang_format(${APP_EXAMPLE_DIR})
else()

    # Create an installable target
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif()
