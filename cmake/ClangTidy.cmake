function(run_clang_tidy DIRECTORY NAME)
  find_program(CLANG_TIDY clang-tidy)

  if(CLANG_TIDY)
    # Find all C++ files in the source directory
    file(GLOB_RECURSE CPP_FILES ${DIRECTORY}/*.cpp)

    # Find all header files in the include directory
    file(GLOB_RECURSE HEADER_FILES ${DIRECTORY}/*.hpp)

    # Concatenate the C++ and header files
    set(ALL_FILES ${CPP_FILES} ${HEADER_FILES})
    add_custom_command(
      TARGET ${NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue
              "Running clang-tidy on all sources in ${DIRECTORY}"
      COMMAND ${CMAKE_COMMAND} -E echo ""
      COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} -header-filter=.*
              --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy ${ALL_FILES})

  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endfunction()
