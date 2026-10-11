#!/usr/bin/env python3
"""
@file storage_orchestration_bridge.py
@brief Python-to-C Bridge for the Storage & System Orchestration Domain - Zero External APIs
"""

import sys
import subprocess
import os

STORAGE_STATE_NODE = "./storage_state_node"
ORCHESTRATION_COMPILER_CORE = "./system_orchestration_compiler"

def invoke_storage_state() -> str:
    """Invokes the compiled C local state persistence and storage engine node."""
    if not os.path.exists(STORAGE_STATE_NODE):
        return "Error: Storage state binary not found. Compile storage_state_node.c first."
    
    try:
        result = subprocess.run(
            [STORAGE_STATE_NODE],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Storage State Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute storage state process: {str(e)}"

def invoke_orchestration_compiler(threads: int, mem_limit: int, mode: str) -> str:
    """Invokes the C system orchestration and storage policy compiler node."""
    if not os.path.exists(ORCHESTRATION_COMPILER_CORE):
        return "Error: Orchestration compiler binary not found. Compile system_orchestration_compiler.c first."
    
    try:
        result = subprocess.run(
            [ORCHESTRATION_COMPILER_CORE, str(threads), str(mem_limit), mode],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Orchestration Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute orchestration compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 storage_orchestration_bridge.py <run_storage|compile_orchestration> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_storage":
        print(invoke_storage_state())
    elif action == "compile_orchestration":
        th = int(sys.argv[2]) if len(sys.argv) > 2 else 8
        ml = int(sys.argv[3]) if len(sys.argv) > 3 else 4096
        mo = sys.argv[4] if len(sys.argv) > 4 else "Deterministic_Multi_Core"
        print(invoke_orchestration_compiler(th, ml, mo))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
