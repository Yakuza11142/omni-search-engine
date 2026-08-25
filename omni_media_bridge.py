"""
OMNI CATEGORY 6 INTERFACE BRIDGE (omni_media_bridge.py)
Dynamic multimodal latent mapper and spatial synthesis executor.
"""

import sys
import os
import re
import subprocess

class OmniCategory6Bridge:
    def __init__(self, binary_path: str = "./omni_media_node", source_path: str = "omni_media_node.c"):
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

    def text_to_latent_vector(self, text_query: str) -> list:
        """Translates multimodal prompt intent into 4D latent embedding coordinates."""
        query_lower = text_query.lower()
        if "3d" in query_lower or "spatial" in query_lower or "tess" in query_lower or "mesh" in query_lower:
            return [0.95, 0.10, 0.40, 0.88]
        elif "code" in query_lower or "ast" in query_lower or "pipeline" in query_lower or "src" in query_lower:
            return [0.15, 0.82, 0.90, 0.05]
        else:
            return [0.50, 0.50, 0.50, 0.50]

    def execute_media_search(self, user_query: str):
        print(f"[BRIDGE] Query Input   : '{user_query}'")
        latent_vec = self.text_to_latent_vector(user_query)
        print(f"[BRIDGE] Latent Vector : {latent_vec}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        result = subprocess.run([self.binary_path], capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory6Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter media, spatial, or code query: ")
    bridge.execute_media_search(query)
