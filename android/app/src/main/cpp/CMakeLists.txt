cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

find_library(log-lib log)

# Directly reference the deterministic memory node source file from the repository root
# Since CMAKE_CURRENT_SOURCE_DIR is android/app/src/main/cpp, 
# we go up 4 levels to reach the repository root where deterministic_memory_nexus_node.c is located.
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../deterministic_memory_nexus_node.c"
)

# Debug check to confirm the file path resolves correctly during build configuration
message(STATUS "Target source file: ${SOURCE_FILES}")

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
