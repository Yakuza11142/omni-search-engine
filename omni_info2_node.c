/**
 * @file omni_info_node.c
 * @brief Information & Memory Vault Node - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAULT_FILE "omni_vault.dat"
#define MAX_LINE_LENGTH 1024

// Initialize the flat-file storage vault
int omni_info_init_vault() {
    FILE *file = fopen(VAULT_FILE, "a+");
    if (!file) {
        fprintf(stderr, "Error: Could not initialize memory vault file.\n");
        return 0;
    }
    fclose(file);
    return 1;
}

// Store a key-value pair permanently with zero external dependencies
int omni_info_store(const char *key, const char *value) {
    if (!key || !value) return 0;

    FILE *file = fopen(VAULT_FILE, "r");
    FILE *temp = fopen("omni_vault_temp.dat", "w");
    
    if (!temp) {
        if (file) fclose(file);
        return 0;
    }

    int found = 0;
    char line[MAX_LINE_LENGTH];

    if (file) {
        while (fgets(line, sizeof(line), file)) {
            char *delim = strchr(line, '=');
            if (delim) {
                *delim = '\0';
                if (strcmp(line, key) == 0) {
                    fprintf(temp, "%s=%s\n", key, value);
                    found = 1;
                } else {
                    *delim = '=';
                    fputs(line, temp);
                }
            } else {
                fputs(line, temp);
            }
        }
        fclose(file);
    }

    if (!found) {
        fprintf(temp, "%s=%s\n", key, value);
    }

    fclose(temp);
    remove(VAULT_FILE);
    rename("omni_vault_temp.dat", VAULT_FILE);
    return 1;
}

// Retrieve exact records from local storage
char* omni_info_retrieve(const char *key) {
    if (!key) return NULL;

    FILE *file = fopen(VAULT_FILE, "r");
    if (!file) return NULL;

    char line[MAX_LINE_LENGTH];
    char *result = NULL;

    while (fgets(line, sizeof(line), file)) {
        char *delim = strchr(line, '=');
        if (delim) {
            *delim = '\0';
            if (strcmp(line, key) == 0) {
                char *newline = strchr(delim + 1, '\n');
                if (newline) *newline = '\0';
                
                result = strdup(delim + 1);
                break;
            }
        }
    }

    fclose(file);
    return result; // Caller must free
}

int main() {
    if (omni_info_init_vault()) {
        printf("Omni Info Pure C Memory Vault initialized successfully (Zero External APIs).\n");
    }
    return 0;
}
