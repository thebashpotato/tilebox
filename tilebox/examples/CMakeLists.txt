#
# Set up peripheral logger
#
set(PERIPHERAL_LOGGER "${PROJECT_NAME}-peripheral-logger-example")
add_executable(${PERIPHERAL_LOGGER} "peripheral_logger.cpp")

target_include_directories(${PERIPHERAL_LOGGER} PUBLIC ${PACKAGE_INCLUDE_DIR})

target_link_libraries(
  ${PERIPHERAL_LOGGER}
  PRIVATE
  tilebox_workspace::tilebox_options
  tilebox_workspace::tilebox_warnings
  fmt::fmt
  etl::etl
  ${PROJECT_NAME})
