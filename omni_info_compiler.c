/*
 * OMNI CATEGORY 1: KNOWLEDGE COMPILER (omni_info_compiler.c)
 * Packages raw facts into a binary spatial vector file (.obx).
 */

#include <unistd.h>
#include <fcntl.h>

#define MAX_FACT_TITLE 64
#define MAX_FACT_BODY  512

typedef struct {
    char magic[4];          // "INFO"
    unsigned int total_facts;
    unsigned int dimensions; // 4D
} KnowledgeHeader;

typedef struct {
    unsigned int fact_id;
    float spatial_vector[4];
    float math_certainty;
    char title[MAX_FACT_TITLE];
    char synthesis[MAX_FACT_BODY];
} KnowledgeFactNode;

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
    int fd = open("category1_knowledge.obx", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 1;

    // Build Header
    KnowledgeHeader header = { {'I', 'N', 'F', 'O'}, 2, 4 };
    write(fd, &header, sizeof(KnowledgeHeader));

    // Sample Fact Node 1 (Physics/Calculus)
    KnowledgeFactNode fact1;
    fact1.fact_id = 101;
    fact1.spatial_vector[0] = 0.12f;
    fact1.spatial_vector[1] = 0.44f;
    fact1.spatial_vector[2] = 0.85f;
    fact1.spatial_vector[3] = 0.05f;
    fact1.math_certainty = 0.999f;
    omni_strcpy(fact1.title, "First Law of Motion", MAX_FACT_TITLE);
    omni_strcpy(fact1.synthesis, "An object remains at rest or in uniform motion unless acted upon by a net external force.", MAX_FACT_BODY);
    write(fd, &fact1, sizeof(KnowledgeFactNode));

    // Sample Fact Node 2 (Computer Systems)
    KnowledgeFactNode fact2;
    fact2.fact_id = 102;
    fact2.spatial_vector[0] = 0.88f;
    fact2.spatial_vector[1] = 0.12f;
    fact2.spatial_vector[2] = 0.33f;
    fact2.spatial_vector[3] = 0.90f;
    fact2.math_certainty = 1.000f;
    omni_strcpy(fact2.title, "Zero-Copy Memory Mapping", MAX_FACT_TITLE);
    omni_strcpy(fact2.synthesis, "mmap maps file descriptors directly into virtual memory pages, avoiding kernel-to-userland buffer copies.", MAX_FACT_BODY);
    write(fd, &fact2, sizeof(KnowledgeFactNode));

    close(fd);
    write(1, "[COMPILER] Built category1_knowledge.obx successfully.\n", 55);
    return 0;
}
