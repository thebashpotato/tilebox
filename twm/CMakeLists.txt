project(
  tilebox_wm
  VERSION 0.1.0
  DESCRIPTION "Tilebox powered Tiling Window Manager (Non-Reparenting)"
  LANGUAGES CXX)

set(TARGET_NAME twm)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Add all source files
#
set(SOURCE_FILES
  "${PACKAGE_SOURCE_DIR}/main.cpp"
  "${PACKAGE_SOURCE_DIR}/log.hpp"
  "${PACKAGE_SOURCE_DIR}/log.cpp"
  "${PACKAGE_SOURCE_DIR}/wm.hpp"
  "${PACKAGE_SOURCE_DIR}/wm.cpp"
  "${PACKAGE_SOURCE_DIR}/xerror_handler.hpp"
  "${PACKAGE_SOURCE_DIR}/xerror_handler.cpp"
  "${PACKAGE_SOURCE_DIR}/atom_manager.cpp")

#
# Output build information
#
message(STATUS "Building target '${TARGET_NAME}' in project '${PROJECT_NAME}' in [${CMAKE_BUILD_TYPE}] mode")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Tests active for project '${PROJECT_NAME}'")
  add_subdirectory(tests)
endif ()

#
# Declare the executable and add all source modules
#
add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(
  ${TARGET_NAME} PRIVATE tilebox_workspace::tilebox_options
  tilebox_workspace::tilebox_warnings)

target_link_system_libraries(${TARGET_NAME} PRIVATE
  ${X11_X11_LIB}
  fmt::fmt
  spdlog::spdlog
  etl::etl
  tilebox::tilebox)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif ()
