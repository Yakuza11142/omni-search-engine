/*
 * OMNI CATEGORY 3: SPATIAL COMPILER (omni_spatial_compiler.c)
 * Packages locations and 4D spatial vector coordinates into binary (.obx).
 */

#include <unistd.h>
#include <fcntl.h>

#define MAX_LOC_NAME 64
#define MAX_LOC_DESC 128

typedef struct {
    char magic[4];          // "SPAT"
    unsigned int total_nodes;
    unsigned int dimensions; // 4D Vector Space
} SpatialHeader;

typedef struct {
    unsigned int location_id;
    float coords[4];         // [Latitude/X, Longitude/Y, Altitude/Z, Category_Vector/W]
    char name[MAX_LOC_NAME];
    char description[MAX_LOC_DESC];
} SpatialNode;

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
    int fd = open("category3_spatial.obx", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 1;

    SpatialHeader header = { {'S', 'P', 'A', 'T'}, 2, 4 };
    write(fd, &header, sizeof(SpatialHeader));

    // Spatial Node 1: Local Tech Hub / Workspace
    SpatialNode node1;
    node1.location_id = 301;
    node1.coords[0] = 9.0765f;  // Latitude X
    node1.coords[1] = 7.3985f;  // Longitude Y
    node1.coords[2] = 450.0f;   // Altitude Z (meters)
    node1.coords[3] = 0.15f;    // Category Vector W (Workspace/Office)
    omni_strcpy(node1.name, "Central Tech Hub", MAX_LOC_NAME);
    omni_strcpy(node1.description, "Primary spatial development and compute center.", MAX_LOC_DESC);
    write(fd, &node1, sizeof(SpatialNode));

    // Spatial Node 2: Coffee & Microlearning Spot
    SpatialNode node2;
    node2.location_id = 302;
    node2.coords[0] = 9.0800f;  // Latitude X
    node2.coords[1] = 7.4000f;  // Longitude Y
    node2.coords[2] = 455.0f;   // Altitude Z
    node2.coords[3] = 0.85f;    // Category Vector W (Amenities/Food)
    omni_strcpy(node2.name, "Quantum Brew Station", MAX_LOC_NAME);
    omni_strcpy(node2.description, "High-speed wireless microlearning lounge.", MAX_LOC_DESC);
    write(fd, &node2, sizeof(SpatialNode));

    close(fd);
    write(1, "[COMPILER] Built category3_spatial.obx successfully.\n", 53);
    return 0;
}
