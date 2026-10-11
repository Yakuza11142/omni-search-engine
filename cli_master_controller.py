#!/usr/bin/env python3
"""
@file cli_master_controller.py
@brief Interactive Command-Line Master Controller Interface - Ecosystem Manager (Zero Dependencies)
"""

import os
import subprocess
import sys

def print_menu():
    print("\n==================================================")
    print("          AI ENGINE CLI MASTER CONTROLLER         ")
    print("==================================================")
    print("1. Run Master Build Orchestrator")
    print("2. Run Automated Integration Test Suite")
    print("3. Run Cross-Platform Packager")
    print("4. Invoke Specific Domain Bridge")
    print("5. Exit")
    print("==================================================")

def main():
    while True:
        print_menu()
        choice = input("Select an option (1-5): ").strip()
        
        if choice == "1":
            subprocess.run([sys.executable, "master_build_orchestrator.py"])
        elif choice == "2":
            subprocess.run([sys.executable, "integration_test_suite.py"])
        elif choice == "3":
            subprocess.run([sys.executable, "cross_platform_packager.py"])
        elif choice == "4":
            print("\nAvailable Domain Bridges:")
            bridges = [f for f in os.listdir(".") if f.endswith("_bridge.py")]
            if not bridges:
                print("  [-] No domain bridges found in current directory.")
                continue
            for i, b in enumerate(bridges, 1):
                print(f"  {i}. {b}")
            
            b_choice = input("Select bridge number to execute: ").strip()
            try:
                b_idx = int(b_choice) - 1
                if 0 <= b_idx < len(bridges):
                    selected_bridge = bridges[b_idx]
                    arg = input(f"Enter arguments for {selected_bridge} (e.g. action arg1 arg2): ").strip()
                    cmd = [sys.executable, selected_bridge]
                    if arg:
                        cmd.extend(arg.split())
                    print(f"\nExecuting: {' '.join(cmd)}")
                    subprocess.run(cmd)
                else:
                    print("  [-] Invalid bridge selection index.")
            except ValueError:
                print("  [-] Please enter a valid numerical choice.")
        elif choice == "5":
            print("Exiting Master Controller. Goodbye!")
            sys.exit(0)
        else:
            print("  [-] Invalid option. Please choose between 1 and 5.")

if __name__ == "__main__":
    main()
