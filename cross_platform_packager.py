#!/usr/bin/env python3
"""
@file cross_platform_packager.py
@brief Cross-Platform Packaging & Distribution Script - Bundles Binaries & Bridges (Zero Dependencies)
"""

import os
import shutil
import sys

DIST_DIR = "./dist_bundle"
TARGETS = ["android_arm64", "linux_x86_64", "wasm_staging"]

def package_ecosystem():
    print("==================================================")
    print("Starting Cross-Platform Ecosystem Packager")
    print("==================================================")

    if os.path.exists(DIST_DIR):
        shutil.rmtree(DIST_DIR)
    
    os.makedirs(DIST_DIR, exist_ok=True)

    for target in TARGETS:
        target_path = os.path.join(DIST_DIR, target)
        os.makedirs(target_path, exist_ok=True)
        os.makedirs(os.path.join(target_path, "bin"), exist_ok=True)
        os.makedirs(os.path.join(target_path, "bridges"), exist_ok=True)
        print(f"  [+] Initialized packaging partition for: {target}")

    # Gather and distribute binaries and bridges
    bundled_binaries = 0
    bundled_bridges = 0

    for item in os.listdir("."):
        # If it's a compiled binary (no extension or executable without .c/.py)
        if os.path.isfile(item) and not item.endswith(('.c', '.py', '.txt', '.md', '.sh')):
            for target in TARGETS:
                dest = os.path.join(DIST_DIR, target, "bin", item)
                shutil.copy2(item, dest)
            bundled_binaries += 1
        # If it's a Python bridge
        elif item.endswith("_bridge.py") or item == "master_build_orchestrator.py":
            for target in TARGETS:
                dest = os.path.join(DIST_DIR, target, "bridges", item)
                shutil.copy2(item, dest)
            bundled_bridges += 1

    print("==================================================")
    print(f"Packaging Complete: {bundled_binaries} binaries and {bundled_bridges} bridges")
    print(f"Distributed across targets in directory: {DIST_DIR}/")
    print("==================================================")

if __name__ == "__main__":
    package_ecosystem()
