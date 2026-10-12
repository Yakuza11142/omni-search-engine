cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

# Locate Android log library
find_library(log-lib log)

# Look one level up into the root directory for all .c files
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../../../*.c")

# Compile all discovered C files into the shared library
add_library(
    memory_node
    SHARED
    ${SOURCE_FILES}
)

# Apply performance optimizations (-O3) and warnings
target_compile_options(memory_node PRIVATE -O3 -Wall)

# Link native system libraries
target_link_libraries(
    memory_node
    ${log-lib}
    android
)
