#!/usr/bin/env python3
"""
@file integration_test_suite.py
@brief Automated End-to-End Integration Test Suite - Validates all 9 domains (Zero Dependencies)
"""

import os
import subprocess
import sys

# Define test targets across all 9 domains
TEST_TARGETS = [
    ("Domain 1: Memory", "./memory_node"),
    ("Domain 2: Compute", "./compute_node"),
    ("Domain 3: Media", "./media_node"),
    ("Domain 4: Navigation", "./navigation_node"),
    ("Domain 5: Swarm", "./swarm_consensus_node"),
    ("Domain 6: Security", "./crypto_shield_node"),
    ("Domain 7: Neural Core", "./neural_tensor_node"),
    ("Domain 8: Holographic UI", "./holographic_ui_node"),
    ("Domain 9: Storage State", "./storage_state_node")
]

def run_integration_tests():
    print("==================================================")
    print("Starting Automated End-to-End Integration Tests")
    print("==================================================")

    passed_tests = 0
    failed_tests = 0

    for domain_name, binary in TEST_TARGETS:
        print(f"\n[Testing {domain_name}] -> Binary: {binary}")
        if not os.path.exists(binary):
            print(f"  [-] FAILED: Binary '{binary}' not found. Run master build first.")
            failed_tests += 1
            continue

        try:
            result = subprocess.run(
                [binary],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                timeout=5
            )
            if result.returncode == 0:
                print(f"  [+] PASSED: Executed cleanly.")
                passed_tests += 1
            else:
                print(f"  [-] FAILED: Non-zero exit code ({result.returncode}).")
                print(f"      Stderr: {result.stderr.strip()}")
                failed_tests += 1
        except Exception as e:
            print(f"  [-] FAILED: Exception during execution: {str(e)}")
            failed_tests += 1

    print("\n==================================================")
    print(f"Test Suite Summary: {passed_tests} Passed, {failed_tests} Failed")
    print("==================================================")

    if failed_tests > 0:
        sys.exit(1)
    else:
        sys.exit(0)

if __name__ == "__main__":
    run_integration_tests()
