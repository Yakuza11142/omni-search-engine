cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

# Locate Android log library
find_library(log-lib log)

# Compile C source code into a shared library matching your Kotlin load call
add_library(
    memory_node
    SHARED
    deterministic_memory_nexus_node.c
)

# Apply performance optimizations (-O3)
target_compile_options(memory_node PRIVATE -O3 -Wall)

# Link native system libraries
target_link_libraries(
    memory_node
    ${log-lib}
    android
)
