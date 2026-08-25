/*
 * OMNI CATEGORY 2: NAVIGATIONAL COMPILER (omni_nav_compiler.c)
 * Packages domain targets and URLs into a compressed hash map binary.
 */

#include <unistd.h>
#include <fcntl.h>

#define MAX_URL_LEN 128
#define MAX_NAME_LEN 64

typedef struct {
    char magic[4];          // "NAVI"
    unsigned int total_nodes;
} NavHeader;

typedef struct {
    unsigned long long node_hash; // 64-bit domain hash ID
    char site_name[MAX_NAME_LEN];
    char primary_url[MAX_URL_LEN];
    int is_secure;                // 1 for HTTPS, 0 for HTTP
} NavNode;

// Simple string hasher (djb2 algorithm) for URL keys
unsigned long long hash_domain(const char *str) {
    unsigned long long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

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
    int fd = open("category2_nav.obx", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 1;

    NavHeader header = { {'N', 'A', 'V', 'I'}, 2 };
    write(fd, &header, sizeof(NavHeader));

    // Nav Node 1
    NavNode node1;
    omni_strcpy(node1.site_name, "GitHub Repositories", MAX_NAME_LEN);
    omni_strcpy(node1.primary_url, "https://github.com/", MAX_URL_LEN);
    node1.node_hash = hash_domain("github");
    node1.is_secure = 1;
    write(fd, &node1, sizeof(NavNode));

    // Nav Node 2
    NavNode node2;
    omni_strcpy(node2.site_name, "Developer Documentation", MAX_NAME_LEN);
    omni_strcpy(node2.primary_url, "https://devdocs.io/", MAX_URL_LEN);
    node2.node_hash = hash_domain("devdocs");
    node2.is_secure = 1;
    write(fd, &node2, sizeof(NavNode));

    close(fd);
    write(1, "[COMPILER] Built category2_nav.obx successfully.\n", 49);
    return 0;
}
