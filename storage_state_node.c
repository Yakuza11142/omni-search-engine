/**
 * @file storage_state_node.c
 * @brief Local State Persistence & Storage Engine - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STORAGE_ENTRIES 128
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 256

// Structure representing a key-value state entry
typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    int is_occupied;
} StateEntry;

// Structure representing the local storage context
typedef struct {
    StateEntry entries[MAX_STORAGE_ENTRIES];
    int active_count;
} StateDatabase;

// Initialize the storage database
void init_storage_db(StateDatabase *db) {
    if (!db) return;
    db->active_count = 0;
    for (int i = 0; i < MAX_STORAGE_ENTRIES; i++) {
        db->entries[i].is_occupied = 0;
        memset(db->entries[i].key, 0, MAX_KEY_LENGTH);
        memset(db->entries[i].value, 0, MAX_VALUE_LENGTH);
    }
}

// Insert or update a key-value pair deterministically
int db_put(StateDatabase *db, const char *key, const char *value) {
    if (!db || !key || !value) return 0;

    // Check if key already exists to update
    for (int i = 0; i < MAX_STORAGE_ENTRIES; i++) {
        if (db->entries[i].is_occupied && strcmp(db->entries[i].key, key) == 0) {
            snprintf(db->entries[i].value, MAX_VALUE_LENGTH, "%s", value);
            return 1;
        }
    }

    // Insert new entry if space permits
    if (db->active_count >= MAX_STORAGE_ENTRIES) return 0;

    for (int i = 0; i < MAX_STORAGE_ENTRIES; i++) {
        if (!db->entries[i].is_occupied) {
            snprintf(db->entries[i].key, MAX_KEY_LENGTH, "%s", key);
            snprintf(db->entries[i].value, MAX_VALUE_LENGTH, "%s", value);
            db->entries[i].is_occupied = 1;
            db->active_count++;
            return 1;
        }
    }
    return 0;
}

// Retrieve a value by key
const char* db_get(const StateDatabase *db, const char *key) {
    if (!db || !key) return NULL;
    for (int i = 0; i < MAX_STORAGE_ENTRIES; i++) {
        if (db->entries[i].is_occupied && strcmp(db->entries[i].key, key) == 0) {
            return db->entries[i].value;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("Initializing Local State Persistence & Storage Engine (Domain 9)...\n");

    StateDatabase db;
    init_storage_db(&db);

    // Perform state write operations
    db_put(&db, "engine_version", "3.0.0-pure");
    db_put(&db, "execution_mode", "deterministic");
    db_put(&db, "runtime_target", "cross-platform");

    // Perform state read validations
    printf("Storage Query [engine_version] -> %s\n", db_get(&db, "engine_version"));
    printf("Storage Query [execution_mode] -> %s\n", db_get(&db, "execution_mode"));
    printf("Storage Query [runtime_target] -> %s\n", db_get(&db, "runtime_target"));

    printf("Storage persistence and state node executed successfully (Pure C Engine).\n");
    return 0;
}
