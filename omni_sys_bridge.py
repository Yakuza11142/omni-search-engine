"""
OMNI CATEGORY 8 INTERFACE BRIDGE (omni_sys_bridge.py)
Dynamic OS query parser and native system administration executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory8Bridge:
    def __init__(self, binary_path: str = "./omni_sys_node", source_path: str = "omni_sys_node.c"):
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

    def parse_sys_opcode(self, text_query: str) -> int:
        """Translates system admin intent into targeted kernel opcodes."""
        query_lower = text_query.lower()
        if "ram" in query_lower or "memory" in query_lower or "process" in query_lower:
            return 1  # Opcode 1: RAM & Memory Diagnostics
        elif "disk" in query_lower or "storage" in query_lower or "nvme" in query_lower:
            return 2  # Opcode 2: Disk & Storage Operations
        return 0      # Opcode 0: Full System Scan

    def execute_sys_diag(self, user_query: str):
        print(f"[BRIDGE] System Query  : '{user_query}'")
        opcode = self.parse_sys_opcode(user_query)
        print(f"[BRIDGE] Mapped Opcode : {opcode}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path, str(opcode)]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory8Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter OS system admin query: ")
    bridge.execute_sys_diag(query)
