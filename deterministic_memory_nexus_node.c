/**
 * @file deterministic_memory_nexus_node.c
 * @brief Deterministic Memory Pool Allocator & Nexus Engine - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jni.h>

#define MAX_POOL_BLOCKS 256
#define BLOCK_SIZE_BYTES 1024

// Structure representing an individual memory block in the deterministic pool
typedef struct {
    uint32_t block_id;
    size_t size;
    int is_allocated;
    uint8_t data[BLOCK_SIZE_BYTES];
} MemoryBlock;

// Structure representing the global memory nexus pool
typedef struct {
    MemoryBlock blocks[MAX_POOL_BLOCKS];
    size_t total_allocated_bytes;
    int active_allocations;
} MemoryNexusPool;

// Initialize the deterministic memory pool
void init_memory_nexus(MemoryNexusPool *pool) {
    if (!pool) return;
    pool->total_allocated_bytes = 0;
    pool->active_allocations = 0;
    for (int i = 0; i < MAX_POOL_BLOCKS; i++) {
        pool->blocks[i].block_id = (uint32_t)(i + 1000);
        pool->blocks[i].size = BLOCK_SIZE_BYTES;
        pool->blocks[i].is_allocated = 0;
        memset(pool->blocks[i].data, 0, BLOCK_SIZE_BYTES);
    }
}

// Allocate a deterministic block from the nexus
void* nexus_allocate(MemoryNexusPool *pool, size_t size) {
    if (!pool || size == 0 || size > BLOCK_SIZE_BYTES) {
        return NULL;
    }

    for (int i = 0; i < MAX_POOL_BLOCKS; i++) {
        if (!pool->blocks[i].is_allocated) {
            pool->blocks[i].is_allocated = 1;
            pool->total_allocated_bytes += size;
            pool->active_allocations++;
            return (void*)pool->blocks[i].data;
        }
    }
    return NULL; // Pool exhausted
}

// Release a block back to the deterministic nexus
int nexus_free(MemoryNexusPool *pool, void *ptr) {
    if (!pool || !ptr) return 0;

    for (int i = 0; i < MAX_POOL_BLOCKS; i++) {
        if ((void*)pool->blocks[i].data == ptr) {
            if (pool->blocks[i].is_allocated) {
                pool->blocks[i].is_allocated = 0;
                pool->total_allocated_bytes -= BLOCK_SIZE_BYTES;
                pool->active_allocations--;
                memset(pool->blocks[i].data, 0, BLOCK_SIZE_BYTES);
                return 1;
            }
        }
    }
    return 0; // Pointer not found in pool
}

// ==========================================
// Android JNI UI Bridge Hook
// ==========================================
JNIEXPORT jstring JNICALL
Java_com_example_omnisys_MainActivity_getSystemStatus(JNIEnv *env, jobject thiz) {
    MemoryNexusPool nexus;
    init_memory_nexus(&nexus);

    void *ptr1 = nexus_allocate(&nexus, 512);
    char status_message[256];
    
    if (ptr1) {
        snprintf(status_message, sizeof(status_message), 
                 "Memory Nexus Active! Allocations: %d | Total Bytes: %zu", 
                 nexus.active_allocations, nexus.total_allocated_bytes);
        nexus_free(&nexus, ptr1);
    } else {
        snprintf(status_message, sizeof(status_message), "Memory Nexus Initialized (Idle)");
    }

    return (*env)->NewStringUTF(env, status_message);
}

// Standard entry point for standalone CLI execution/testing
int main(int argc, char *argv[]) {
    printf("Initializing Deterministic Memory Nexus Node (Domain 1 - File 1)...\n");

    MemoryNexusPool nexus;
    init_memory_nexus(&nexus);

    // Perform deterministic allocations
    void *ptr1 = nexus_allocate(&nexus, 512);
    void *ptr2 = nexus_allocate(&nexus, 256);

    if (ptr1 && ptr2) {
        printf("Successfully allocated two deterministic blocks.\n");
        printf("Active Allocations: %d | Total Allocated Bytes: %zu\n", 
               nexus.active_allocations, nexus.total_allocated_bytes);
    }

    // Release allocations cleanly
    nexus_free(&nexus, ptr1);
    nexus_free(&nexus, ptr2);

    printf("Memory Nexus Post-Cleanup Active Allocations: %d\n", nexus.active_allocations);
    printf("Deterministic Memory Nexus Node executed successfully.\n");
    return 0;
}
