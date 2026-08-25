"""
OMNI CATEGORY 3 INTERFACE BRIDGE (omni_spatial_bridge.py)
Dynamic spatial coordinate translator and proximity search executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory3Bridge:
    def __init__(self, binary_path: str = "./omni_spatial_node", source_path: str = "omni_spatial_node.c"):
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

    def parse_spatial_input(self, text_query: str) -> list:
        """Extracts numerical vector coordinates or maps spatial keywords dynamically."""
        # Find explicit floats/integers in prompt
        numbers = re.findall(r"[-+]?\d*\.\d+|\d+", text_query)
        if len(numbers) >= 4:
            return [float(n) for n in numbers[:4]]

        # Dynamic vector fallback based on domain classification
        query_lower = text_query.lower()
        if "coffee" in query_lower or "food" in query_lower or "lounge" in query_lower:
            return [9.0800, 7.4000, 455.0, 0.85]
        else:
            # Default location origin vector
            return [9.0770, 7.3990, 451.0, 0.15]

    def execute_spatial_search(self, user_query: str):
        print(f"[BRIDGE] Query Input   : '{user_query}'")
        coords = self.parse_spatial_input(user_query)
        print(f"[BRIDGE] Target Vector : {coords}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path] + [str(c) for c in coords]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory3Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter spatial query or coordinates: ")
    bridge.execute_spatial_search(query)
