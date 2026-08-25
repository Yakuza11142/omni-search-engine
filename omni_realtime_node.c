/*
 * OMNI CATEGORY 5: REAL-TIME RUNTIME NODE (omni_realtime_node.c)
 * Zero-dependency stream reader and live telemetry scanner.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_FEED_NAME 64
#define MAX_EVENT_MSG 128

typedef struct {
    char magic[4];
    unsigned int head_idx;
    unsigned int total_events;
} RealtimeHeader;

typedef struct {
    unsigned long long timestamp;
    unsigned int stream_id;
    char source[MAX_FEED_NAME];
    char payload[MAX_EVENT_MSG];
} RealtimeEventNode;

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

int omni_strstr(const char *haystack, const char *needle) {
    if (!*needle) return 1;
    for (; *haystack; haystack++) {
        const char *h = haystack, *n = needle;
        while (*h && *n && (*h == *n)) { h++; n++; }
        if (!*n) return 1;
    }
    return 0;
}

void omni_query_realtime(const char *obx_filepath, const char *filter_keyword) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] Real-time stream buffer missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    RealtimeHeader *header = (RealtimeHeader *)mapped_mem;
    RealtimeEventNode *events = (RealtimeEventNode *)((char *)mapped_mem + sizeof(RealtimeHeader));

    omni_write_str("\n--- CATEGORY 5 REAL-TIME & LIVE STREAM SCAN ---\n");

    int matches = 0;
    for (unsigned int i = 0; i < header->total_events; i++) {
        if (filter_keyword[0] == '\0' || omni_strstr(events[i].source, filter_keyword) || omni_strstr(events[i].payload, filter_keyword)) {
            matches++;
            omni_write_str("SOURCE : ");
            omni_write_str(events[i].source);
            omni_write_str("\nPAYLOAD: ");
            omni_write_str(events[i].payload);
            omni_write_str("\n-----------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No active stream events matched the target filter.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    const char *filter = (argc > 1) ? argv[1] : "";
    omni_query_realtime("category5_realtime.obx", filter);
    return 0;
}
