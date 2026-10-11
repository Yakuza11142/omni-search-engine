#!/usr/bin/env python3
"""
@file media_sync_stream_bridge.py
@brief Python-to-C Bridge for the Media & 8K Rendering Domain - Zero External APIs
"""

import sys
import subprocess
import os

MEDIA_PROCESSOR_NODE = "./media_frame_processor"
STREAM_OPTIMIZER_CORE = "./pixel_stream_optimizer"

def invoke_media_processor() -> str:
    """Invokes the compiled C 8K frame buffer allocation and processing node."""
    if not os.path.exists(MEDIA_PROCESSOR_NODE):
        return "Error: Media processor binary not found. Compile media_frame_processor.c first."
    
    try:
        result = subprocess.run(
            [MEDIA_PROCESSOR_NODE],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=10
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Media Processing Execution Error: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute media processor process: {str(e)}"

def invoke_stream_optimizer(width: int, height: int, fps: int, codec: str) -> str:
    """Invokes the C media stream optimization and validation gate."""
    if not os.path.exists(STREAM_OPTIMIZER_CORE):
        return "Error: Stream optimizer binary not found. Compile pixel_stream_optimizer.c first."
    
    try:
        result = subprocess.run(
            [STREAM_OPTIMIZER_CORE, str(width), str(height), str(fps), codec],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Optimization Validation Failed: {result.stderr.strip()}"
    except Exception as e:
        return f"Failed to execute stream optimizer process: {str(e)}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 media_sync_stream_bridge.py <process_frame|optimize_stream> [args...]")
        sys.exit(1)
        
    action = sys.argv[1]
    
    if action == "process_frame":
        print(invoke_media_processor())
    elif action == "optimize_stream":
        w = int(sys.argv[2]) if len(sys.argv) > 2 else 7680
        h = int(sys.argv[3]) if len(sys.argv) > 3 else 4320
        f = int(sys.argv[4]) if len(sys.argv) > 4 else 60
        c = sys.argv[5] if len(sys.argv) > 5 else "RAW_RGBA_STREAM"
        print(invoke_stream_optimizer(w, h, f, c))
    else:
        print("Invalid bridge action specified.")
        sys.exit(1)
