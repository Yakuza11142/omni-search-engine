"""
OMNI CATEGORY 5 INTERFACE BRIDGE (omni_realtime_bridge.py)
Dynamic real-time stream query interceptor and terminal executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory5Bridge:
    def __init__(self, binary_path: str = "./omni_realtime_node", source_path: str = "omni_realtime_node.c"):
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

    def parse_realtime_filter(self, text_query: str) -> str:
        """Strips filler words and extracts stream filters."""
        query_clean = re.sub(r'^(?:show|get|fetch|check|live|realtime|stream|current|status)\s+', '', text_query.strip(), flags=re.IGNORECASE)
        return query_clean.strip()

    def execute_realtime_search(self, user_query: str):
        print(f"[BRIDGE] Query Input   : '{user_query}'")
        filter_str = self.parse_realtime_filter(user_query)
        print(f"[BRIDGE] Stream Filter : '{filter_str}'")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path, filter_str]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory5Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter real-time query or stream topic: ")
    bridge.execute_realtime_search(query)
