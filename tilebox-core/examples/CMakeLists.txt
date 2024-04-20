#
# Set up scratch file
#
set(SCRATCH_FILE "${PROJECT_NAME}-scratch")
add_executable(${SCRATCH_FILE} "scratch.cpp")

target_include_directories(${SCRATCH_FILE} PUBLIC ${APP_INCLUDE_DIR})

target_link_libraries(
    ${SCRATCH_FILE}
    PRIVATE tilebox_workspace_project_options
            tilebox_workspace_project_warnings
            stdc++fs
            fmt
            ${PROJECT_NAME})
