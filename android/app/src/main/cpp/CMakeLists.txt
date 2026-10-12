cmake_minimum_required(VERSION 3.22.1)
project("memory_node")

find_library(log-lib log)

# Explicitly point four levels up from android/app/src/main/cpp to the root directory
file(GLOB SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../deterministic_memory_nexus_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_sys_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omnisys_*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crypto_shield_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../quantum_calc_engine.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../spatial_routing_compiler.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../storage_state_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../swarm_consensus_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../truth_validator_core.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../vector_path_router.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../media_frame_processor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../neural_tensor_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_nav_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_realtime_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_spatial_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_swarm_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_comm_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_compute_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_info_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../omni_media_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../holographic_ui_node.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../matrix_transform_compiler.c"
)

# Fallback wildcard glob if specific files need to be caught
if(NOT SOURCE_FILES)
    file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../../../*.c")
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
