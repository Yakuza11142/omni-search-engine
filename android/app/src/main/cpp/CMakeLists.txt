cmake_minimum_required(VERSION 3.22.1)
project("omni_engine")

# Locate Android log library
find_library(log-lib log)

# Compile C source code into a shared library
add_library(
    omni_engine
    SHARED
    main.c
)

# Apply performance optimizations (-O3)
target_compile_options(omni_engine PRIVATE -O3 -Wall)

# Link native system libraries
target_link_libraries(
    omni_engine
    ${log-lib}
    android
)
