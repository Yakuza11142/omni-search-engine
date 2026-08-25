/*
 * OMNI CATEGORY 2: ROUTING RUNTIME NODE (omni_nav_node.c)
 * Zero-dependency URL hash map router.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_URL_LEN 128
#define MAX_NAME_LEN 64

typedef struct {
    char magic[4];
    unsigned int total_nodes;
} NavHeader;

typedef struct {
    unsigned long long node_hash;
    char site_name[MAX_NAME_LEN];
    char primary_url[MAX_URL_LEN];
    int is_secure;
} NavNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

// Re-implement djb2 hash to match compiler
unsigned long long hash_domain(const char *str) {
    unsigned long long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

void omni_route_nav(const char *obx_filepath, const char *target_domain) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Category 2 binary dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    NavHeader *header = (NavHeader *)mapped_mem;
    NavNode *nodes = (NavNode *)((char *)mapped_mem + sizeof(NavHeader));

    unsigned long long target_hash = hash_domain(target_domain);
    int match_found = 0;

    omni_write_str("\n--- CATEGORY 2 NAVIGATIONAL ROUTING ---\n");

    for (unsigned int i = 0; i < header->total_nodes; i++) {
        if (nodes[i].node_hash == target_hash) {
            match_found = 1;
            omni_write_str("DESTINATION: ");
            omni_write_str(nodes[i].site_name);
            omni_write_str("\nENDPOINT   : ");
            omni_write_str(nodes[i].primary_url);
            omni_write_str("\nPROTOCOL   : ");
            omni_write_str(nodes[i].is_secure ? "HTTPS (Secure)" : "HTTP");
            omni_write_str("\n---------------------------------------\n");
            break; 
        }
    }

    if (!match_found) {
        omni_write_str("No direct domain route found in memory map.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

// Entry point expects a domain string as an argument, defaults to "github" for testing
int main(int argc, char *argv[]) {
    const char *query = (argc > 1) ? argv[1] : "github";
    omni_route_nav("category2_nav.obx", query);
    return 0;
}
