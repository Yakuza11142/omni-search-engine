/*
 * OMNI CATEGORY 6: MEDIA RUNTIME NODE (omni_media_node.c)
 * Zero-dependency spatial latent match & asset resolver.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_ASSET_NAME 64
#define MAX_ASSET_PATH 128

typedef struct {
    char magic[4];
    unsigned int total_assets;
    unsigned int latent_dims;
} MediaHeader;

typedef struct {
    unsigned int asset_id;
    float latent_vector[4];
    unsigned int asset_type;
    char name[MAX_ASSET_NAME];
    char resource_path[MAX_ASSET_PATH];
} MediaAssetNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

float compute_latent_distance(const float *v1, const float *v2) {
    float d0 = v1[0] - v2[0];
    float d1 = v1[1] - v2[1];
    float d2 = v1[2] - v2[2];
    float d3 = v1[3] - v2[3];
    return (d0 * d0) + (d1 * d1) + (d2 * d2) + (d3 * d3);
}

const char* get_type_label(unsigned int type_id) {
    switch (type_id) {
        case 1: return "3D Spatial Mesh (.tess)";
        case 2: return "Audio Waveform Signal";
        case 3: return "Synthesized Code AST";
        default: return "Generic Multimodal Asset";
    }
}

void omni_query_media(const char *obx_filepath, const float query_latent[4]) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Category 6 binary dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    MediaHeader *header = (MediaHeader *)mapped_mem;
    MediaAssetNode *assets = (MediaAssetNode *)((char *)mapped_mem + sizeof(MediaHeader));

    omni_write_str("\n--- CATEGORY 6 MULTIMODAL & SPATIAL SYNTHESIS ---\n");

    for (unsigned int i = 0; i < header->total_assets; i++) {
        float dist = compute_latent_distance(query_latent, assets[i].latent_vector);

        if (dist < 0.30f) {
            omni_write_str("ASSET NAME: ");
            omni_write_str(assets[i].name);
            omni_write_str("\nTYPE       : ");
            omni_write_str(get_type_label(assets[i].asset_type));
            omni_write_str("\nPATH / URI : ");
            omni_write_str(assets[i].resource_path);
            omni_write_str("\n-------------------------------------------------\n");
        }
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main() {
    // Latent query vector representing spatial / 3D model query intent
    float sample_latent[4] = {0.95f, 0.10f, 0.40f, 0.88f};
    omni_query_media("category6_media.obx", sample_latent);
    return 0;
}
