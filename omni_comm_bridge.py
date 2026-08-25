"""
OMNI CATEGORY 4 INTERFACE BRIDGE (omni_comm_bridge.py)
Dynamic price parser and product specification filter executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory4Bridge:
    def __init__(self, binary_path: str = "./omni_comm_node", source_path: str = "omni_comm_node.c"):
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

    def parse_commercial_input(self, text_query: str) -> tuple:
        """Extracts max price ceiling and feature bitmask flags dynamically."""
        # Find explicit currency numbers (e.g. "under 1000", "$500")
        prices = re.findall(r"\$?\b\d+(?:\.\d{1,2})?\b", text_query)
        max_price = float(prices[0].replace("$", "")) if prices else 2000.0

        flags = 0x01 # Base default: Portable flag
        query_lower = text_query.lower()
        if "high-performance" in query_lower or "power" in query_lower or "workstation" in query_lower:
            flags |= 0x04

        return max_price, flags

    def execute_commercial_search(self, user_query: str):
        print(f"[BRIDGE] Query Input    : '{user_query}'")
        max_price, flags = self.parse_commercial_input(user_query)
        print(f"[BRIDGE] Max Price Limit: ${max_price:.2f} | Flags Bitmask: {hex(flags)}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path, str(max_price), str(flags)]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory4Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter commercial search or price filter: ")
    bridge.execute_commercial_search(query)
