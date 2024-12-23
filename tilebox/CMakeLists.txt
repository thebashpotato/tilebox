project(
  tilebox
  VERSION 0.1.0
  DESCRIPTION
    "Provides a modern C++ RAII wrapper around X11 objects and idioms, as well as providing a very basic drawing and font utilities"
  LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PACKAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Add all source files
#
set(SOURCE_FILES
    "${PACKAGE_SOURCE_DIR}/geometry.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/display.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/window.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/events.cpp"
    "${PACKAGE_SOURCE_DIR}/x11/event_loop.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/font.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/utf8_codec.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/draw.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/color.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/colorscheme.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/colorscheme_config.cpp"
    "${PACKAGE_SOURCE_DIR}/draw/cursor.cpp")

#
# Output build information
#
message(
  STATUS "Building project '${PROJECT_NAME}' in [${CMAKE_BUILD_TYPE}] mode")

#
# Add tests if in Debug mode
#
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Tests active for project '${PROJECT_NAME}'")
  add_subdirectory(tests)
endif()

#
# Generate configuration file
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure_files/config.hpp.in
               ${PACKAGE_INCLUDE_DIR}/${PROJECT_NAME}/config.hpp)

#
# Create the library
#
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_library(tilebox::tilebox ALIAS ${PROJECT_NAME})

# Link required dependencies
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE tilebox_workspace::tilebox_options
          tilebox_workspace::tilebox_warnings
          fmt::fmt
          spdlog::spdlog
          ${X11_Xft_LIB}
          ${X11_X11_LIB}
          ${Fontconfig_LIBRARY})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${PACKAGE_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>
         ${X11_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR_ft2build}
         ${FREETYPE_INCLUDE_DIR_freetype2} ${Fontconfig_INCLUDE_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  run_clang_format(${PACKAGE_INCLUDE_DIR})
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif()
