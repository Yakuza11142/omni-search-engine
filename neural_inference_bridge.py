#!/usr/bin/env python3
"""
@file neural_inference_bridge.py
@brief Python-to-C Bridge for the Neural Core & Inference Domain - Zero External APIs
"""

import sys
import subprocess
import os

NEURAL_TENSOR_NODE = "./neural_tensor_node"
GRAPH_COMPILER_CORE = "./neural_graph_compiler"

def invoke_neural_tensor() -> str:
    """Invokes the compiled C deterministic neural tensor inference node."""
    if not os.path.exists(NEURAL_TENSOR_NODE):
        return "Error: Neural tensor binary not found. Compile neural_tensor_node.c first."
    
    try:
        result = subprocess.run(
            [NEURAL_TENSOR_NODE],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Neural Tensor Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute neural tensor process: {str(e)}"

def invoke_graph_compiler(layers: int, quant_bits: int, backend: str) -> str:
    """Invokes the C neural network architecture and graph compiler node."""
    if not os.path.exists(GRAPH_COMPILER_CORE):
        return "Error: Graph compiler binary not found. Compile neural_graph_compiler.c first."
    
    try:
        result = subprocess.run(
            [GRAPH_COMPILER_CORE, str(layers), str(quant_bits), backend],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Graph Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute graph compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 neural_inference_bridge.py <run_tensor|compile_graph> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_tensor":
        print(invoke_neural_tensor())
    elif action == "compile_graph":
        l = int(sys.argv[2]) if len(sys.argv) > 2 else 24
        q = int(sys.argv[3]) if len(sys.argv) > 3 else 32
        b = sys.argv[4] if len(sys.argv) > 4 else "SIMD_Native_CPU"
        print(invoke_graph_compiler(l, q, b))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
