"""
OMNI CATEGORY 2 INTERFACE BRIDGE (omni_nav_bridge.py)
Dynamic intent extractor and C runtime router. Zero hardcoded domains or fixed queries.
"""

import sys
import os
import re
import subprocess

class OmniCategory2Bridge:
    def __init__(self, binary_path: str = "./omni_nav_node", source_path: str = "omni_nav_node.c"):
        self.binary_path = binary_path
        self.source_path = source_path
        
        # Dynamic regex pattern for conversational intent prefixes
        self.nav_intent_pattern = re.compile(
            r'^(?:can\s+you\s+)?(?:please\s+)?(?:go\s+to|open|take\s+me\s+to|navigate\s+to|show|find)\s+',
            re.IGNORECASE
        )

    def _ensure_binary_exists(self) -> bool:
        """Auto-compiles C source file if compiled binary node is missing."""
        if os.path.exists(self.binary_path):
            return True
        
        if os.path.exists(self.source_path):
            print(f"[BRIDGE] Binary missing. Auto-compiling '{self.source_path}'...")
            build = subprocess.run(["gcc", "-O3", self.source_path, "-o", self.binary_path])
            return build.returncode == 0
            
        return False

    def extract_domain(self, text_query: str) -> str:
        """Extracts dynamic multi-word targets without relying on static arrays."""
        # Strip common lead-in navigational phrases dynamically
        cleaned_query = self.nav_intent_pattern.sub("", text_query.strip())
        
        # Strip trailing filler phrases (e.g., "for me", "please")
        cleaned_query = re.sub(r'\s+(?:for\s+me|please|website|site)$', '', cleaned_query, flags=re.IGNORECASE)
        
        # Remove non-alphanumeric trailing/leading characters except dots/hyphens
        target = re.sub(r'^[^\w]+|[^\w\.-]+$', '', cleaned_query.lower())
        
        return target if target else "unknown"

    def execute_routing(self, text_query: str) -> str:
        """Executes zero-dependency sub-process call to C hash engine."""
        target_domain = self.extract_domain(text_query)
        print(f"[BRIDGE] Query Input    : '{text_query}'")
        print(f"[BRIDGE] Target Extracted: '{target_domain}'")

        if not self._ensure_binary_exists():
            print(f"[ERROR] C Runtime binary target ('{self.binary_path}') not found or failed to compile.")
            return ""

        # Invoke C node passing dynamic target domain string as CLI arg
        result = subprocess.run([self.binary_path, target_domain], capture_output=True, text=True)
        print(result.stdout)
        return result.stdout


if __name__ == "__main__":
    bridge = OmniCategory2Bridge()
    
    # Accept dynamic query from command-line args or fallback to interactive terminal prompt
    if len(sys.argv) > 1:
        user_input = " ".join(sys.argv[1:])
    else:
        user_input = input("Enter navigational query: ")
        
    bridge.execute_routing(user_input)
