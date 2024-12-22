include(cmake/SystemLink.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(tilebox_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES
                                                   ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES
                                                   ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(tilebox_setup_options)
  option(tilebox_ENABLE_HARDENING "Enable hardening" ON)
  option(tilebox_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    tilebox_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies" ON
    tilebox_ENABLE_HARDENING OFF)

  tilebox_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR tilebox_PACKAGING_MAINTAINER_MODE)
    option(tilebox_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(tilebox_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(tilebox_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(tilebox_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(tilebox_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(tilebox_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(tilebox_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(tilebox_ENABLE_PCH "Enable precompiled headers" OFF)
    option(tilebox_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(tilebox_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(tilebox_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(tilebox_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(tilebox_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer"
           ${SUPPORTS_ASAN})
    option(tilebox_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer"
           ${SUPPORTS_UBSAN})
    option(tilebox_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(tilebox_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(tilebox_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(tilebox_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(tilebox_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(tilebox_ENABLE_PCH "Enable precompiled headers" OFF)
    option(tilebox_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      tilebox_ENABLE_IPO
      tilebox_WARNINGS_AS_ERRORS
      tilebox_ENABLE_USER_LINKER
      tilebox_ENABLE_SANITIZER_ADDRESS
      tilebox_ENABLE_SANITIZER_LEAK
      tilebox_ENABLE_SANITIZER_UNDEFINED
      tilebox_ENABLE_SANITIZER_THREAD
      tilebox_ENABLE_SANITIZER_MEMORY
      tilebox_ENABLE_UNITY_BUILD
      tilebox_ENABLE_CLANG_TIDY
      tilebox_ENABLE_CPPCHECK
      tilebox_ENABLE_COVERAGE
      tilebox_ENABLE_PCH
      tilebox_ENABLE_CACHE)
  endif()
endmacro()

macro(tilebox_global_options)
  if(tilebox_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    tilebox_enable_ipo()
  endif()

  tilebox_supports_sanitizers()

  if(tilebox_ENABLE_HARDENING AND tilebox_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR tilebox_ENABLE_SANITIZER_UNDEFINED
       OR tilebox_ENABLE_SANITIZER_ADDRESS
       OR tilebox_ENABLE_SANITIZER_THREAD
       OR tilebox_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message(
      "${tilebox_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${tilebox_ENABLE_SANITIZER_UNDEFINED}"
    )
    tilebox_enable_hardening(tilebox_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(tilebox_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(tilebox_warnings INTERFACE)
  add_library(tilebox_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  set_tilebox_warnings(tilebox_warnings ${tilebox_WARNINGS_AS_ERRORS} "" "" ""
                       "")

  if(tilebox_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    tilebox_configure_linker(tilebox_options)
  endif()

  include(cmake/Sanitizers.cmake)
  tilebox_enable_sanitizers(
    tilebox_options ${tilebox_ENABLE_SANITIZER_ADDRESS}
    ${tilebox_ENABLE_SANITIZER_LEAK} ${tilebox_ENABLE_SANITIZER_UNDEFINED}
    ${tilebox_ENABLE_SANITIZER_THREAD} ${tilebox_ENABLE_SANITIZER_MEMORY})

  set_target_properties(tilebox_options
                        PROPERTIES UNITY_BUILD ${tilebox_ENABLE_UNITY_BUILD})

  if(tilebox_ENABLE_PCH)
    target_precompile_headers(tilebox_options INTERFACE <vector> <string>
                              <utility>)
  endif()

  if(tilebox_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    tilebox_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(tilebox_ENABLE_CLANG_TIDY)
    tilebox_enable_clang_tidy(tilebox_options ${tilebox_WARNINGS_AS_ERRORS})
  endif()

  if(tilebox_ENABLE_CPPCHECK)
    tilebox_enable_cppcheck(
      ${tilebox_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(tilebox_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    tilebox_enable_coverage(tilebox_options)
  endif()

  if(tilebox_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(tilebox_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(tilebox_ENABLE_HARDENING AND NOT tilebox_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR tilebox_ENABLE_SANITIZER_UNDEFINED
       OR tilebox_ENABLE_SANITIZER_ADDRESS
       OR tilebox_ENABLE_SANITIZER_THREAD
       OR tilebox_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    tilebox_enable_hardening(tilebox_options OFF
                             ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()
