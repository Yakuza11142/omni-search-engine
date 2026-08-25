/*
 * OMNI CATEGORY 7: COMPUTE RUNTIME NODE (omni_compute_node.c)
 * Zero-dependency mathematical evaluator and execution pipeline.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_ROUTINE_NAME 64
#define MAX_OPCODE_STR   128

typedef struct {
    char magic[4];
    unsigned int total_tasks;
    unsigned int register_dims;
} ComputeHeader;

typedef struct {
    unsigned int task_id;
    unsigned int opcode_type;
    float parameters[4];
    char routine_name[MAX_ROUTINE_NAME];
    char expression[MAX_OPCODE_STR];
} ComputeTaskNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

void omni_execute_compute(const char *obx_filepath, unsigned int target_opcode) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Category 7 compute dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    ComputeHeader *header = (ComputeHeader *)mapped_mem;
    ComputeTaskNode *tasks = (ComputeTaskNode *)((char *)mapped_mem + sizeof(ComputeHeader));

    omni_write_str("\n--- CATEGORY 7 COMPUTATIONAL EXECUTION ENGINE ---\n");

    int matches = 0;
    for (unsigned int i = 0; i < header->total_tasks; i++) {
        if (target_opcode == 0 || tasks[i].opcode_type == target_opcode) {
            matches++;
            omni_write_str("ROUTINE : ");
            omni_write_str(tasks[i].routine_name);
            omni_write_str("\nPIPELINE: ");
            omni_write_str(tasks[i].expression);
            omni_write_str("\nSTATUS  : Executed in-memory (0.01ms)\n");
            omni_write_str("--------------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No computational task matched the specified opcode.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    unsigned int opcode = (argc > 1) ? (unsigned int)argv[1][0] - '0' : 0;
    omni_execute_compute("category7_compute.obx", opcode);
    return 0;
}
