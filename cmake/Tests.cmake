function(tilebox_enable_coverage tilebox_name)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES
                                             ".*Clang")

    target_compile_options(${tilebox_name} INTERFACE --coverage -O0 -g)
    target_link_libraries(${tilebox_name} INTERFACE --coverage)
  endif()
endfunction()
