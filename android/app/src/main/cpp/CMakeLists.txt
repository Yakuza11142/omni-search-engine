cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

find_library(log-lib log)

# Get the absolute path of the repository root directory 
# (going up 4 levels from android/app/src/main/cpp)
get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../" ABSOLUTE)

# Explicitly gather the C files from the absolute root directory path
file(GLOB SOURCE_FILES 
    "${REPO_ROOT}/deterministic_memory_nexus_node.c"
    "${REPO_ROOT}/omni_sys_node.c"
    "${REPO_ROOT}/omnisys_*.c"
    "${REPO_ROOT}/crypto_shield_node.c"
    "${REPO_ROOT}/quantum_calc_engine.c"
    "${REPO_ROOT}/spatial_routing_compiler.c"
    "${REPO_ROOT}/storage_state_node.c"
    "${REPO_ROOT}/swarm_consensus_node.c"
    "${REPO_ROOT}/truth_validator_core.c"
    "${REPO_ROOT}/vector_path_router.c"
    "${REPO_ROOT}/media_frame_processor.c"
    "${REPO_ROOT}/neural_tensor_node.c"
    "${REPO_ROOT}/omni_nav_node.c"
    "${REPO_ROOT}/omni_realtime_node.c"
    "${REPO_ROOT}/omni_spatial_node.c"
    "${REPO_ROOT}/omni_swarm_node.c"
    "${REPO_ROOT}/omni_comm_node.c"
    "${REPO_ROOT}/omni_compute_node.c"
    "${REPO_ROOT}/omni_info_node.c"
    "${REPO_ROOT}/omni_media_node.c"
    "${REPO_ROOT}/holographic_ui_node.c"
    "${REPO_ROOT}/matrix_transform_compiler.c"
)

# Fallback wildcard to grab any remaining .c files in the root if needed
if(NOT SOURCE_FILES)
    file(GLOB SOURCE_FILES "${REPO_ROOT}/*.c")
endif()

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
