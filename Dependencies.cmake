include(cmake/CPM.cmake)

# Done as a function so that updates to variables like CMAKE_CXX_FLAGS don't
# propagate out to other targets
function(tilebox_setup_dependencies)
  # NOTE: https://cmake.org/cmake/help/latest/module/FindX11.html
  find_package(X11 REQUIRED)

  # NOTE: https://cmake.org/cmake/help/latest/module/FindFontconfig.html
  find_package(Fontconfig REQUIRED)

  # NOTE: https://cmake.org/cmake/help/latest/module/FindFreetype.html
  find_package(Freetype REQUIRED)

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (NOT TARGET gtest)
      cpmaddpackage(NAME
        googletest
        GITHUB_REPOSITORY
        google/googletest
        GIT_TAG
        v1.15.2
        VERSION
        1.15.2
        OPTIONS
        "BUILD_GMOCK ON"
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON")
    endif ()
  endif ()

  if (NOT TARGET etl::etl)
    cpmaddpackage("gh:thebashpotato/etl#v0.8.4")
  endif ()

  if (NOT TARGET utf8cpp)
    cpmaddpackage("gh:nemtrif/utfcpp#v4.0.6")
  endif ()

  # For each dependency, see if it's already been provided to us by a parent
  # project
  if (NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#11.0.2")
  endif ()

  if (NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
      spdlog
      VERSION
      1.14.1
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif ()
endfunction()
