"""
OMNI CATEGORY 10 INTERFACE BRIDGE (omni_swarm_bridge.py)
Dynamic agent consensus parser and native swarm runtime executor.
"""

import sys
import os
import subprocess

class OmniCategory10Bridge:
    def __init__(self, binary_path: str = "./omni_swarm_node", source_path: str = "omni_swarm_node.c"):
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

    def parse_agent_status(self, text_query: str) -> int:
        """Translates multi-agent intent into target status codes."""
        query_lower = text_query.lower()
        if "leader" in query_lower or "master" in query_lower or "primary" in query_lower:
            return 1  # Status 1: Primary Leader Nodes
        elif "worker" in query_lower or "follower" in query_lower or "replica" in query_lower:
            return 2  # Status 2: Worker Replica Nodes
        return 0      # Status 0: Full Swarm Audit

    def execute_swarm_audit(self, user_query: str):
        print(f"[BRIDGE] Swarm Query  : '{user_query}'")
        status = self.parse_agent_status(user_query)
        print(f"[BRIDGE] Mapped Status Filter : {status}")

        if not self._ensure_binary_exists():
            print(f"[ERROR] Binary '{self.binary_path}' not found.")
            return

        cmd = [self.binary_path, str(status)]
        result = subprocess.run(cmd, capture_output=True, text=True)
        print(result.stdout)

if __name__ == "__main__":
    bridge = OmniCategory10Bridge()
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else input("Enter swarm or multi-agent query: ")
    bridge.execute_swarm_audit(query)
