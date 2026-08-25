/*
 * OMNI CATEGORY 3: SPATIAL RUNTIME NODE (omni_spatial_node.c)
 * Zero-dependency 4D spatial vector proximity scanner.
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_LOC_NAME 64
#define MAX_LOC_DESC 128

typedef struct {
    char magic[4];
    unsigned int total_nodes;
    unsigned int dimensions;
} SpatialHeader;

typedef struct {
    unsigned int location_id;
    float coords[4];
    char name[MAX_LOC_NAME];
    char description[MAX_LOC_DESC];
} SpatialNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

// 4D Euclidean Distance calculation: sqrt((dx)^2 + (dy)^2 + (dz)^2 + (dw)^2)
float compute_spatial_distance(const float *c1, const float *c2) {
    float dx = c1[0] - c2[0];
    float dy = c1[1] - c2[1];
    float dz = (c1[2] - c2[2]) / 1000.0f; // Scale elevation to coordinate ratio
    float dw = c1[3] - c2[3];
    return (dx * dx) + (dy * dy) + (dz * dz) + (dw * dw);
}

void omni_query_spatial(const char *obx_filepath, const float user_coords[4], float radius_threshold) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Spatial binary map missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    SpatialHeader *header = (SpatialHeader *)mapped_mem;
    SpatialNode *nodes = (SpatialNode *)((char *)mapped_mem + sizeof(SpatialHeader));

    omni_write_str("\n--- CATEGORY 3 SPATIAL & GEOLOCATION SCAN ---\n");

    int matches = 0;
    for (unsigned int i = 0; i < header->total_nodes; i++) {
        float dist_sq = compute_spatial_distance(user_coords, nodes[i].coords);

        if (dist_sq <= radius_threshold) {
            matches++;
            omni_write_str("LOCATION   : ");
            omni_write_str(nodes[i].name);
            omni_write_str("\nDESCRIPTION: ");
            omni_write_str(nodes[i].description);
            omni_write_str("\n---------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No spatial targets located within current vector radius.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    // Default origin query coordinates [Lat, Lon, Alt, Category]
    float user_coords[4] = {9.0770f, 7.3990f, 451.0f, 0.15f};
    float radius_threshold = 0.05f;

    // Overridden dynamically if CLI args are supplied
    if (argc >= 5) {
        user_coords[0] = atof(argv[1]);
        user_coords[1] = atof(argv[2]);
        user_coords[2] = atof(argv[3]);
        user_coords[3] = atof(argv[4]);
    }

    omni_query_spatial("category3_spatial.obx", user_coords, radius_threshold);
    return 0;
}
