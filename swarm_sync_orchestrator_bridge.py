#!/usr/bin/env python3
"""
@file swarm_sync_orchestrator_bridge.py
@brief Python-to-C Bridge for the Swarm Intelligence & Synchronization Domain - Zero External APIs
"""

import sys
import subprocess
import os

SWARM_NODE_ENGINE = "./swarm_consensus_node"
PROTOCOL_COMPILER_CORE = "./distributed_protocol_compiler"

def invoke_swarm_consensus(node_count: int) -> str:
    """Invokes the compiled C decentralized swarm node consensus engine."""
    if not os.path.exists(SWARM_NODE_ENGINE):
        return "Error: Swarm node binary not found. Compile swarm_consensus_node.c first."
    
    try:
        result = subprocess.run(
            [SWARM_NODE_ENGINE, str(node_count)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Swarm Consensus Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute swarm consensus process: {str(e)}"

def invoke_protocol_compiler(heartbeat_ms: int, tolerance: float, algorithm: str) -> str:
    """Invokes the C distributed protocol and topology compiler."""
    if not os.path.exists(PROTOCOL_COMPILER_CORE):
        return "Error: Protocol compiler binary not found. Compile distributed_protocol_compiler.c first."
    
    try:
        result = subprocess.run(
            [PROTOCOL_COMPILER_CORE, str(heartbeat_ms), str(tolerance), algorithm],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Protocol Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute protocol compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 swarm_sync_orchestrator_bridge.py <run_swarm|compile_protocol> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_swarm":
        count = int(sys.argv[2]) if len(sys.argv) > 2 else 8
        print(invoke_swarm_consensus(count))
    elif action == "compile_protocol":
        hb = int(sys.argv[2]) if len(sys.argv) > 2 else 250
        tol = float(sys.argv[3]) if len(sys.argv) > 3 else 0.33
        algo = sys.argv[4] if len(sys.argv) > 4 else "Raft_Paxos_Hybrid_Consensus"
        print(invoke_protocol_compiler(hb, tol, algo))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
