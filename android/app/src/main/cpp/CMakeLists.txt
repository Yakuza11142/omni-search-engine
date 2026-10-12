cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

find_library(log-lib log)

# Use the absolute path passed from Gradle, falling back safely if needed
if(NOT DEFINED PROJECT_ROOT_DIR)
    get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../" ABSOLUTE)
endif()

# Automatically grab every single .c file from the repository root directory
file(GLOB SOURCE_FILES "${PROJECT_ROOT_DIR}/*.c")

# Debug check to confirm files are found during build configuration
message(STATUS "Looking for C files in: ${PROJECT_ROOT_DIR}")
message(STATUS "Found source files: ${SOURCE_FILES}")

add_library(
    memory_node
    SHARED
    ${SOURCE_FILES}
)

target_compile_options(memory_node PRIVATE -O3 -Wall)

target_link_libraries(
    memory_node
    ${log-lib}
    android
)
