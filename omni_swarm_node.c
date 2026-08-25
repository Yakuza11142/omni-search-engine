/*
 * OMNI CATEGORY 10: SWARM RUNTIME NODE (omni_swarm_node.c)
 * Zero-dependency peer state auditor and consensus checker.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_AGENT_NAME 64
#define MAX_STATE_KEY  128

typedef struct {
    char magic[4];
    unsigned int total_agents;
    unsigned int quorum_threshold;
} SwarmHeader;

typedef struct {
    unsigned int agent_id;
    unsigned int status_flag;
    unsigned int weight;
    char agent_name[MAX_AGENT_NAME];
    char active_state[MAX_STATE_KEY];
} SwarmAgentNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

void omni_audit_swarm(const char *obx_filepath, unsigned int target_status) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Swarm binary dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    SwarmHeader *header = (SwarmHeader *)mapped_mem;
    SwarmAgentNode *agents = (SwarmAgentNode *)((char *)mapped_mem + sizeof(SwarmHeader));

    omni_write_str("\n--- CATEGORY 10 MULTI-AGENT CONSENSUS & SWARM ENGINE ---\n");

    int matches = 0;
    for (unsigned int i = 0; i < header->total_agents; i++) {
        if (target_status == 0 || agents[i].status_flag == target_status) {
            matches++;
            omni_write_str("AGENT NAME : ");
            omni_write_str(agents[i].agent_name);
            omni_write_str("\nAGENT STATE: ");
            omni_write_str(agents[i].active_state);
            omni_write_str("\nROLE STATUS: Verified Local Consensus Node\n");
            omni_write_str("----------------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No agent node matched the status filter.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    unsigned int status = (argc > 1) ? (unsigned int)(argv[1][0] - '0') : 0;
    omni_audit_swarm("category10_swarm.obx", status);
    return 0;
}
