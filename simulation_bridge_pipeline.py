#!/usr/bin/env python3
"""
@file simulation_bridge_pipeline.py
@brief Python-to-C Bridge for the Compute & Simulation Domain - Zero External APIs
"""

import sys
import subprocess
import os

COMPUTE_NODE = "./quantum_calc_engine"
COMPILER_CORE = "./matrix_transform_compiler"

def run_simulation_node(particle_count: int = 100) -> str:
    """Invokes the compiled C simulation engine natively."""
    if not os.path.exists(COMPUTE_NODE):
        return "Error: Compute node binary not found. Compile quantum_calc_engine.c first."
    
    try:
        result = subprocess.run(
            [COMPUTE_NODE, str(particle_count)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Simulation Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute simulation process: {str(e)}"

def compile_simulation_spec(dimensions: int, sim_type: str) -> str:
    """Invokes the C compute compiler validation gate."""
    if not os.path.exists(COMPILER_CORE):
        return "Error: Compiler binary not found. Compile matrix_transform_compiler.c first."
    
    try:
        result = subprocess.run(
            [COMPILER_CORE, str(dimensions), sim_type],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 simulation_bridge_pipeline.py <run_sim|compile_spec> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_sim":
        count = int(sys.argv[2]) if len(sys.argv) > 2 else 100
        print(run_simulation_node(count))
    elif action == "compile_spec":
        dims = int(sys.argv[2]) if len(sys.argv) > 2 else 3
        stype = sys.argv[3] if len(sys.argv) > 3 else "Newtonian_Physics"
        print(compile_simulation_spec(dims, stype))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
