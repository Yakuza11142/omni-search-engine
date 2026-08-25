/*
 * OMNI CATEGORY 4: COMMERCIAL COMPILER (omni_comm_compiler.c)
 * Packages product specifications and pricing matrices into binary (.obx).
 */

#include <unistd.h>
#include <fcntl.h>

#define MAX_PROD_NAME 64
#define MAX_SPEC_STR  128

typedef struct {
    char magic[4];          // "COMM"
    unsigned int total_items;
    unsigned int spec_dims;  // Number of numeric attribute vectors
} CommHeader;

typedef struct {
    unsigned int product_id;
    float price;             // Normalized numeric price point
    float rating;            // Quality score (0.0 to 5.0)
    unsigned int feature_flags; // Bitmask for quick spec checks
    char name[MAX_PROD_NAME];
    char specs[MAX_SPEC_STR];
} CommProductNode;

int omni_strcpy(char *dest, const char *src, int max_len) {
    int i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int main() {
    int fd = open("category4_commercial.obx", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 1;

    CommHeader header = { {'C', 'O', 'M', 'M'}, 2, 2 };
    write(fd, &header, sizeof(CommHeader));

    // Product Node 1: High-End Hardware Workstation
    CommProductNode prod1;
    prod1.product_id = 401;
    prod1.price = 1499.99f;
    prod1.rating = 4.9f;
    prod1.feature_flags = 0x01 | 0x04; // e.g., Portable | High-Performance
    omni_strcpy(prod1.name, "Omni Neural Compute Unit v1", MAX_PROD_NAME);
    omni_strcpy(prod1.specs, "64-Core Vector Engine, 32GB Unified RAM, Zero-Latency M.2 NVMe Slot.", MAX_SPEC_STR);
    write(fd, &prod1, sizeof(CommProductNode));

    // Product Node 2: Mobile Spatial Rig
    CommProductNode prod2;
    prod2.product_id = 402;
    prod2.price = 499.00f;
    prod2.rating = 4.7f;
    prod2.feature_flags = 0x01 | 0x02; // e.g., Portable | Low-Power
    omni_strcpy(prod2.name, "Tesseract Vector Deck", MAX_PROD_NAME);
    omni_strcpy(prod2.specs, "Mobile Spatial Processing Rig, ARM64 Optimized, Integrated .tess Hardware Accelerator.", MAX_SPEC_STR);
    write(fd, &prod2, sizeof(CommProductNode));

    close(fd);
    write(1, "[COMPILER] Built category4_commercial.obx successfully.\n", 56);
    return 0;
}
