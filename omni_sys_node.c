/*
 * OMNI CATEGORY 8: SYSTEM RUNTIME NODE (omni_sys_node.c)
 * Zero-dependency native kernel interop & process monitor.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>

#define MAX_TARGET_NAME 64
#define MAX_COMMAND_STR 128

typedef struct {
    char magic[4];
    unsigned int total_tasks;
    unsigned int kernel_abi;
} SysHeader;

typedef struct {
    unsigned int task_id;
    unsigned int sys_opcode;
    unsigned int privilege_level;
    char target_name[MAX_TARGET_NAME];
    char syscall_mapping[MAX_COMMAND_STR];
} SysTaskNode;

// Helper to safely retrieve live kernel uptime and status into a buffer
int omni_get_system_runtime_status(char *out_buffer, size_t max_len) {
    struct sysinfo s_info;
    if (sysinfo(&s_info) == 0) {
        snprintf(out_buffer, max_len,
                 "--- CATEGORY 8 SYSTEM RUNTIME ---\n"
                 "LIVE KERNEL UPTIME : %ld seconds\n"
                 "TOTAL RAM          : %lu MB\n"
                 "FREE RAM           : %lu MB\n"
                 "PROCESSES          : %d\n"
                 "Status             : Kernel Interop Active",
                 (long)s_info.uptime,
                 (unsigned long)(s_info.totalram / 1024 / 1024),
                 (unsigned long)(s_info.freeram / 1024 / 1024),
                 s_info.procs);
        return 1;
    } else {
        snprintf(out_buffer, max_len, "Status: Kernel sysinfo query failed.");
        return 0;
    }
}
