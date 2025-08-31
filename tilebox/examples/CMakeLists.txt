#
# Set up peripheral logger
#
set(PERIPHERAL_LOGGER "${PROJECT_NAME}-peripheral-logger-example")
set(SCRATCH "${PROJECT_NAME}-scratch")


add_executable(${PERIPHERAL_LOGGER} "peripheral_logger.cpp")
add_executable(${SCRATCH} "scratch.cpp")

target_include_directories(${PERIPHERAL_LOGGER} PUBLIC ${PACKAGE_INCLUDE_DIR})
target_include_directories(${SCRATCH} PUBLIC ${PACKAGE_INCLUDE_DIR})

target_link_libraries(
  ${PERIPHERAL_LOGGER}
  PRIVATE
  tilebox_workspace::tilebox_options
  tilebox_workspace::tilebox_warnings
  etl::etl
  ${PROJECT_NAME})

target_link_libraries(
  ${SCRATCH}
  PRIVATE
  tilebox_workspace::tilebox_options
  tilebox_workspace::tilebox_warnings
  etl::etl
  ${PROJECT_NAME})
