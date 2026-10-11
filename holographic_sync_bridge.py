#!/usr/bin/env python3
"""
@file holographic_sync_bridge.py
@brief Python-to-C Bridge for the UI & Holographic Display Domain - Zero External APIs
"""

import sys
import subprocess
import os

HOLOGRAPHIC_UI_NODE = "./holographic_ui_node"
LAYOUT_COMPILER_CORE = "./holographic_layout_compiler"

def invoke_holographic_ui(element_count: int) -> str:
    """Invokes the compiled C holographic UI and spatial display rendering node."""
    if not os.path.exists(HOLOGRAPHIC_UI_NODE):
        return "Error: Holographic UI binary not found. Compile holographic_ui_node.c first."
    
    try:
        result = subprocess.run(
            [HOLOGRAPHIC_UI_NODE, str(element_count)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Holographic UI Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute holographic UI process: {str(e)}"

def invoke_layout_compiler(capacity: int, depth_limit: float, shader: str) -> str:
    """Invokes the C holographic layout and spatial component compiler node."""
    if not os.path.exists(LAYOUT_COMPILER_CORE):
        return "Error: Holographic layout compiler binary not found. Compile holographic_layout_compiler.c first."
    
    try:
        result = subprocess.run(
            [LAYOUT_COMPILER_CORE, str(capacity), str(depth_limit), shader],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Layout Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute layout compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 holographic_sync_bridge.py <run_ui|compile_layout> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_ui":
        count = int(sys.argv[2]) if len(sys.argv) > 2 else 4
        print(invoke_holographic_ui(count))
    elif action == "compile_layout":
        cap = int(sys.argv[2]) if len(sys.argv) > 2 else 128
        dl = float(sys.argv[3]) if len(sys.argv) > 3 else 100.0
        sh = sys.argv[4] if len(sys.argv) > 4 else "Pure_C_Ray_Marching_Depth"
        print(invoke_layout_compiler(cap, dl, sh))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
