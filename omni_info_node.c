/*
 * OMNI CATEGORY 1: SEARCH RUNTIME NODE (omni_info_node.c)
 * Zero-dependency spatial vector query scanner.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_FACT_TITLE 64
#define MAX_FACT_BODY  512

typedef struct {
    char magic[4];
    unsigned int total_facts;
    unsigned int dimensions;
} KnowledgeHeader;

typedef struct {
    unsigned int fact_id;
    float spatial_vector[4];
    float math_certainty;
    char title[MAX_FACT_TITLE];
    char synthesis[MAX_FACT_BODY];
} KnowledgeFactNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

float compute_vector_distance(const float *v1, const float *v2) {
    float d0 = v1[0] - v2[0];
    float d1 = v1[1] - v2[1];
    float d2 = v1[2] - v2[2];
    float d3 = v1[3] - v2[3];
    return (d0 * d0) + (d1 * d1) + (d2 * d2) + (d3 * d3);
}

void omni_query_knowledge(const char *obx_filepath, const float query_vector[4]) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Binary dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    KnowledgeHeader *header = (KnowledgeHeader *)mapped_mem;
    KnowledgeFactNode *facts = (KnowledgeFactNode *)((char *)mapped_mem + sizeof(KnowledgeHeader));

    omni_write_str("\n--- CATEGORY 1 INFORMATIONAL SCAN ---\n");

    for (unsigned int i = 0; i < header->total_facts; i++) {
        float dist = compute_vector_distance(query_vector, facts[i].spatial_vector);

        if (dist < 0.25f) {
            omni_write_str("TITLE: ");
            omni_write_str(facts[i].title);
            omni_write_str("\nFACT : ");
            omni_write_str(facts[i].synthesis);
            omni_write_str("\n-------------------------------------\n");
        }
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main() {
    // Input vector for physics/motion query
    float sample_query[4] = {0.12f, 0.44f, 0.85f, 0.05f};
    omni_query_knowledge("category1_knowledge.obx", sample_query);
    return 0;
}
