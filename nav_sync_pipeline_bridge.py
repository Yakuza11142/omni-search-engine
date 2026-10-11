#!/usr/bin/env python3
"""
@file nav_sync_pipeline_bridge.py
@brief Python-to-C Bridge for the Navigation & Routing Domain - Zero External APIs
"""

import sys
import subprocess
import os

VECTOR_ROUTER_NODE = "./vector_path_router"
SPATIAL_COMPILER_CORE = "./spatial_routing_compiler"

def invoke_vector_router() -> str:
    """Invokes the compiled C spatial coordinate and pathfinding router node."""
    if not os.path.exists(VECTOR_ROUTER_NODE):
        return "Error: Vector router binary not found. Compile vector_path_router.c first."
    
    try:
        result = subprocess.run(
            [VECTOR_ROUTER_NODE],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Vector Routing Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute vector router process: {str(e)}"

def invoke_spatial_compiler(node_count: int, range_limit: float, algorithm: str) -> str:
    """Invokes the C navigation topology and spatial routing compiler."""
    if not os.path.exists(SPATIAL_COMPILER_CORE):
        return "Error: Spatial compiler binary not found. Compile spatial_routing_compiler.c first."
    
    try:
        result = subprocess.run(
            [SPATIAL_COMPILER_CORE, str(node_count), str(range_limit), algorithm],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Spatial Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute spatial compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 nav_sync_pipeline_bridge.py <run_router|compile_nav> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_router":
        print(invoke_vector_router())
    elif action == "compile_nav":
        nodes = int(sys.argv[2]) if len(sys.argv) > 2 else 1000
        limit = float(sys.argv[3]) if len(sys.argv) > 3 else 500.0
        algo = sys.argv[4] if len(sys.argv) > 4 else "Dijkstra_Euclidean_Hybrid"
        print(invoke_spatial_compiler(nodes, limit, algo))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
