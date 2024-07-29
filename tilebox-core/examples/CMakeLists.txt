#
# Set up scratch file
#
set(SCRATCH_FILE "${PROJECT_NAME}-scratch-example")
add_executable(${SCRATCH_FILE} "scratch.cpp")

target_include_directories(${SCRATCH_FILE} PUBLIC ${APP_INCLUDE_DIR})

target_link_libraries(
  ${SCRATCH_FILE}
  PRIVATE workspace_project_options
          workspace_project_warnings
          stdc++fs
          fmt
          ${X11_Xft_LIB}
          ${X11_X11_LIB}
          ${Fontconfig_LIBRARY}
          ${PROJECT_NAME})

#
# Set up peripheral logger
#
set(PERIPHERAL_LOGGER "${PROJECT_NAME}-peripheral-logger-example")
add_executable(${PERIPHERAL_LOGGER} "peripheral_logger.cpp")

target_include_directories(${PERIPHERAL_LOGGER} PUBLIC ${APP_INCLUDE_DIR}
                                                       ${X11_INCLUDE_DIR})

target_link_libraries(
  ${PERIPHERAL_LOGGER}
  PRIVATE workspace_project_options workspace_project_warnings stdc++fs fmt
          ${PROJECT_NAME} ${X11_LIBRARIES})
