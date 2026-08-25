/*
 * OMNI CATEGORY 4: COMMERCIAL RUNTIME NODE (omni_comm_node.c)
 * Zero-dependency multi-variable product comparison scanner.
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_PROD_NAME 64
#define MAX_SPEC_STR  128

typedef struct {
    char magic[4];
    unsigned int total_items;
    unsigned int spec_dims;
} CommHeader;

typedef struct {
    unsigned int product_id;
    float price;
    float rating;
    unsigned int feature_flags;
    char name[MAX_PROD_NAME];
    char specs[MAX_SPEC_STR];
} CommProductNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

void omni_query_commercial(const char *obx_filepath, float max_price, unsigned int required_flags) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Commercial binary map missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    CommHeader *header = (CommHeader *)mapped_mem;
    CommProductNode *items = (CommProductNode *)((char *)mapped_mem + sizeof(CommHeader));

    omni_write_str("\n--- CATEGORY 4 COMMERCIAL & PRODUCT COMPARISON ---\n");

    int matches = 0;
    for (unsigned int i = 0; i < header->total_items; i++) {
        // Multi-variable condition: Price ceiling check & feature bitmask check
        if (items[i].price <= max_price && (items[i].feature_flags & required_flags) == required_flags) {
            matches++;
            omni_write_str("PRODUCT : ");
            omni_write_str(items[i].name);
            omni_write_str("\nSPECS   : ");
            omni_write_str(items[i].specs);
            omni_write_str("\n-------------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No items matching target specifications within price limit.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    float max_price = 2000.0f;
    unsigned int required_flags = 0x01; // Default requirement: Portable flag

    if (argc >= 2) {
        max_price = atof(argv[1]);
    }
    if (argc >= 3) {
        required_flags = atoi(argv[2]);
    }

    omni_query_commercial("category4_commercial.obx", max_price, required_flags);
    return 0;
}
