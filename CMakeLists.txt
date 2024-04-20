#
# Minimum required cmake version
#
cmake_minimum_required(VERSION 3.18)

#
# Set up workspace
#
project(
    tilebox_workspace
    VERSION 0.1.0
    DESCRIPTION "A box of X11 programs written for me, by me. Tiling window manager, screen locker, menu, terminal"
    LANGUAGES CXX)

#
# Project Options
#
option(DEVELOPER_MODE "Enables testing and example builds" ON)

#
# Add custom cmake modules
#
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#
# Include all custom modules
#
include(CPM)
include(StandardProjectSettings)
include(StaticAnalyzers)
include(CompilerWarnings)
include(ClangFormat)
include(ClangTidy)

message(STATUS "Building ${PROJECT_NAME} -- ${PROJECT_DESCRIPTION} -- Written in C++${CMAKE_CXX_STANDARD}")

#
# Create interfaced "libraries" for custom StandardProjectSettings and CompilerWarnings
#
add_library(tilebox_workspace_project_options INTERFACE)
target_compile_features(tilebox_workspace_project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(tilebox_workspace_project_warnings INTERFACE)
set_project_warnings(tilebox_workspace_project_warnings)

#
# NOTE: FindPackage and CPM (Dependencies needed by all projects are defined at this level only.
#
# PackageProject.cmake will be used to make our target installable
cpmaddpackage("gh:TheLartians/PackageProject.cmake@1.8.0")

# NOTE: https://cmake.org/cmake/help/latest/module/FindX11.html
find_package(X11 REQUIRED)

# NOTE: https://cmake.org/cmake/help/latest/module/FindFontconfig.html
find_package(Fontconfig REQUIRED)

# NOTE: https://cmake.org/cmake/help/latest/module/FindFreetype.html
find_package(Freetype REQUIRED)

#
# Check for developer mode, add google tests library if it is
#
if(DEVELOPER_MODE)
    set(CMAKE_BUILD_TYPE
        Debug
        CACHE STRING "Forcing build type to Debug" FORCE)

    cpmaddpackage(
        NAME
        googletest
        GITHUB_REPOSITORY
        google/googletest
        GIT_TAG
        v1.14.0
        VERSION
        1.14.0
        OPTIONS
        "BUILD_GMOCK ON"
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON")
    if(googletest_ADDED)
        message(STATUS "gtest configured correctly")
    else()
        message(WARNING "gtest was not configured properly")
    endif()
    enable_testing()
else()
    set(CMAKE_BUILD_TYPE
        Release
        CACHE STRING "Forcing build type to Release" FORCE)
endif()

#
# Add sub-projects
#
add_subdirectory(tilebox-core)
add_subdirectory(window-manager)
