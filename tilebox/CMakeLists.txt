project(
  tilebox
  VERSION 0.1.0
  DESCRIPTION
    "Provides a modern C++ RAII wrapper around Xlib objects and idioms, as well as providing a very basic drawing and font utilities"
  LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PACKAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(PACKAGE_EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

#
# Add all source files
#
set(PACKAGE_PRIVATE_SOURCES
    "${PACKAGE_SOURCE_DIR}/geometry.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/display.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/window.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/events.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/event_loop.cpp"
    "${PACKAGE_SOURCE_DIR}/ui/draw.cpp"
    "${PACKAGE_SOURCE_DIR}/ui/font.hpp"
    "${PACKAGE_SOURCE_DIR}/ui/font.cpp"
    "${PACKAGE_SOURCE_DIR}/ui/font_set.hpp"
    "${PACKAGE_SOURCE_DIR}/ui/font_set.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/font.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/utf8_codec.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/draw.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/color.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/colorscheme.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/colorscheme_config.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/cursor.cpp")

set(PACKAGE_PUBLIC_HEADERS
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/config.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/geometry.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/utils/attributes.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/utils/debug.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/ui/draw.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/x11/display.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/x11/event_loop.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/x11/events.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/x11/window.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/color.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/colorscheme.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/colorscheme_config.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/cursor.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/draw.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/font.hpp"
    "${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/draw/utf8_codec.hpp")

#
# Output build information
#
message(STATUS "Building project '${PROJECT_NAME}' in [${CMAKE_BUILD_TYPE}] mode")

#
# Add tests if in Debug mode
#
if(tilebox_ENABLE_DEVELOPER_MODE)
  message(STATUS "Tests and Examples active for project '${PROJECT_NAME}'")
  include_directories(${PACKAGE_SOURCE_DIR})
  add_subdirectory(tests)
  add_subdirectory(examples)
endif()

#
# Generate configuration file
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure_files/config.hpp.in
               ${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/config.hpp)

#
# Create the library
#
add_library(${PROJECT_NAME})
add_library(tilebox::tilebox ALIAS ${PROJECT_NAME})

#
# Add private sources to the library
#
target_sources(${PROJECT_NAME} PRIVATE ${PACKAGE_PRIVATE_SOURCES})

#
# Declare public header files (will also be made available to IDE) and version the library
#
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES PUBLIC_HEADER "${PACKAGE_PUBLIC_HEADERS}"
             VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

# Link required dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE tilebox_workspace::tilebox_options tilebox_workspace::tilebox_warnings)

target_link_system_libraries(
  ${PROJECT_NAME}
  PRIVATE
  etl::etl
  utf8cpp
  ${X11_Xft_LIB}
  ${X11_X11_LIB}
  ${Fontconfig_LIBRARY})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PACKAGE_INCLUDE_DIR}>
                                                  $<INSTALL_INTERFACE:include>)

target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR_ft2build}
                                                  ${FREETYPE_INCLUDE_DIR_freetype2} ${Fontconfig_INCLUDE_DIR})

if(tilebox_ENABLE_DEVELOPER_MODE)
  run_clang_format(${PACKAGE_INCLUDE_DIR})
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
  run_clang_format(${PACKAGE_EXAMPLE_DIR})
endif()
