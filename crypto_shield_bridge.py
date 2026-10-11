#!/usr/bin/env python3
"""
@file crypto_shield_bridge.py
@brief Python-to-C Bridge for the Security & Cryptography Domain - Zero External APIs
"""

import sys
import subprocess
import os

CRYPTO_SHIELD_NODE = "./crypto_shield_node"
SECURITY_COMPILER_CORE = "./security_audit_compiler"

def invoke_crypto_shield(payload: str) -> str:
    """Invokes the compiled C cryptographic integrity and secure checksum node."""
    if not os.path.exists(CRYPTO_SHIELD_NODE):
        return "Error: Crypto shield binary not found. Compile crypto_shield_node.c first."
    
    try:
        result = subprocess.run(
            [CRYPTO_SHIELD_NODE, payload],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Crypto Shield Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute crypto shield process: {str(e)}"

def invoke_security_compiler(key_length: int, cipher_mode: str, tamper_guard: int) -> str:
    """Invokes the C security policy and compliance compiler node."""
    if not os.path.exists(SECURITY_COMPILER_CORE):
        return "Error: Security audit compiler binary not found. Compile security_audit_compiler.c first."
    
    try:
        result = subprocess.run(
            [SECURITY_COMPILER_CORE, str(key_length), cipher_mode, str(tamper_guard)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Security Compilation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute security compiler process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 crypto_shield_bridge.py <run_shield|compile_policy> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "run_shield":
        pl = sys.argv[2] if len(sys.argv) > 2 else "Secure_Payload_Verification_Token_2026"
        print(invoke_crypto_shield(pl))
    elif action == "compile_policy":
        kl = int(sys.argv[2]) if len(sys.argv) > 2 else 256
        cm = sys.argv[3] if len(sys.argv) > 3 else "AES_256_GCM_Deterministic"
        tg = int(sys.argv[4]) if len(sys.argv) > 4 else 1
        print(invoke_security_compiler(kl, cm, tg))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
