#!/usr/bin/env python3
"""
@file master_build_orchestrator.py
@brief Master Build Automation Script - Compiles all C nodes & compilers across all 9 domains (Zero Dependencies)
"""

import os
import subprocess
import sys

# Define all 9 domains with their respective C node and compiler source files
DOMAINS = [
    ("1_memory", "memory_node.c", "memory_compiler.c"),
    ("2_compute", "compute_node.c", "compute_compiler.c"),
    ("3_media", "media_node.c", "media_compiler.c"),
    ("4_navigation", "navigation_node.c", "navigation_compiler.c"),
    ("5_swarm", "swarm_consensus_node.c", "distributed_protocol_compiler.c"),
    ("6_security", "crypto_shield_node.c", "security_audit_compiler.c"),
    ("7_neural", "neural_tensor_node.c", "neural_graph_compiler.c"),
    ("8_ui", "holographic_ui_node.c", "holographic_layout_compiler.c"),
    ("9_storage", "storage_state_node.c", "system_orchestration_compiler.c")
]

def build_all_domains():
    print("==================================================")
    print("Starting Master Build Orchestrator (Pure C / -O3)")
    print("==================================================")
    
    success_count = 0
    total_targets = len(DOMAINS) * 2

    for domain_name, node_src, compiler_src in DOMAINS:
        print(f"\n[Domain: {domain_name.upper()}]")
        for src in [node_src, compiler_src]:
            if not os.path.exists(src):
                print(f"  [-] Warning: Source file '{src}' not found in current directory.")
                continue
            
            bin_name = src.rsplit('.', 1)[0]
            # Compile with optimizations and standard math library linkage (-O3, -lm)
            cmd = ["gcc", src, "-o", bin_name, "-O3", "-lm"]
            print(f"  [*] Compiling {src} -> {bin_name}...")
            
            result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            if result.returncode == 0:
                print(f"      [+] Built successfully.")
                success_count += 1
            else:
                print(f"      [-] Build failed:\n{result.stderr.strip()}")

    print("\n==================================================")
    print(f"Master Build Complete: {success_count}/{total_targets} binaries ready.")
    print("==================================================")

if __name__ == "__main__":
    build_all_domains()
