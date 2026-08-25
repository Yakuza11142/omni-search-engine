"""
OMNI CATEGORY 1 INTERFACE BRIDGE (omni_info_bridge.py)
Connects high-level queries to the native C binary node.
"""

import subprocess
import os

class OmniCategory1Bridge:
    def __init__(self, binary_path="./omni_info_node"):
        self.binary_path = binary_path

    def text_to_vector(self, text_query: str) -> list:
        """Translates raw query text into 4D spatial vector coordinates."""
        query_lower = text_query.lower()
        if "motion" in query_lower or "physics" in query_lower or "force" in query_lower:
            return [0.12, 0.44, 0.85, 0.05]
        elif "memory" in query_lower or "mmap" in query_lower or "system" in query_lower:
            return [0.88, 0.12, 0.33, 0.90]
        else:
            return [0.50, 0.50, 0.50, 0.50]

    def execute_search(self, text_query: str):
        print(f"[BRIDGE] Query received: '{text_query}'")
        vector = self.text_to_vector(text_query)
        print(f"[BRIDGE] Vector mapped: {vector}")

        if os.path.exists(self.binary_path):
            # Executes low-level binary compiled node directly
            result = subprocess.run([self.binary_path], capture_output=True, text=True)
            print(result.stdout)
        else:
            print("[ERROR] C Runtime binary file ('omni_info_node') not found. Compile it first.")

if __name__ == "__main__":
    bridge = OmniCategory1Bridge()
    bridge.execute_search("Explain physics laws of motion")
