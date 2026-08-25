"""
OMNI CATEGORY 7 INTERFACE BRIDGE (omni_compute_bridge.py)
Dynamic mathematical expression router and native compute executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory7Bridge:
    def __init__(self, binary_path: str = "./omni_compute_node", source_path: str = "omni_compute_node.c"):
        self.binary_path = binary_path
        self.source_path = source_path

    def _ensure_binary_exists(self) -> bool:
        if os.path.exists(self.binary_path):
            return True
        if os.path.exists(self.source_path):
            print(f"[BRIDGE] Compiling '{self.source_path}'...")
            build = subprocess.run(["gcc", "-O3", self.source_path, "-o", self.binary_path])
            return build.returncode == 0
        return False

    def parse_compute_opcode(self, text_query: str) -> int:
        """Maps query intent to specific numerical execution opcodes."""
        query_lower = text_query.lower()
        if "matrix" in query_lower or "transform" in query_lower or "spatial" in query_lower or "vector" in query_lower:
            return 1  # Opcode 1: Spatial Matrix Transform
        elif "prime" in query_lower or "math" in query_lower or "calculus" in query_lower or "eval" in query_lower:
            return 2  # Opcode 2: Mathematical Derivation
        return 0      # Opcode 0: Execute All Pending Tasks

    def execute_compute(self, user_query: str):
        print(f"[BRIDGE] Mathematical Input : '{user_query}'")
        opcode = self.parse_compute_opcode(user_query)
        print(f"[BRIDGE] Mapped Opcode     : {opcode}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path, str(opcode)]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory7Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter mathematical or spatial compute query: ")
    bridge.execute_compute(query)
