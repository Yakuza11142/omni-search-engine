/*
 * OMNI CATEGORY 8: SYSTEM RUNTIME NODE (omni_sys_node.c)
 * Zero-dependency native kernel interop & process monitor.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

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

void omni_write_str(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    write(1, str, len);
}

void omni_execute_sys_diag(const char *obx_filepath, unsigned int target_opcode) {
    int fd = open(obx_filepath, O_RDONLY);
    if (fd < 0) {
        omni_write_str("[ERROR] System binary dataset missing.\n");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) { close(fd); return; }

    void *mapped_mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_mem == MAP_FAILED) { close(fd); return; }

    SysHeader *header = (SysHeader *)mapped_mem;
    SysTaskNode *tasks = (SysTaskNode *)((char *)mapped_mem + sizeof(SysHeader));

    omni_write_str("\n--- CATEGORY 8 SYSTEM ADMIN & KERNEL INTEROP ---\n");

    // Execute direct sysinfo syscall for live system stats
    struct sysinfo s_info;
    if (sysinfo(&s_info) == 0) {
        omni_write_str("LIVE KERNEL UP TIME : ");
        // Print raw uptime seconds
        char uptime_buf[16];
        long ut = s_info.uptime;
        int idx = 0;
        char temp[16]; int t_idx = 0;
        if (ut == 0) temp[t_idx++] = '0';
        while (ut > 0) { temp[t_idx++] = '0' + (ut % 10); ut /= 10; }
        while (t_idx > 0) uptime_buf[idx++] = temp[--t_idx];
        uptime_buf[idx] = '\0';
        omni_write_str(uptime_buf);
        omni_write_str(" seconds\n");
    }

    int matches = 0;
    for (unsigned int i = 0; i < header->total_tasks; i++) {
        if (target_opcode == 0 || tasks[i].sys_opcode == target_opcode) {
            matches++;
            omni_write_str("SUBSYSTEM : ");
            omni_write_str(tasks[i].target_name);
            omni_write_str("\nSYSCALL   : ");
            omni_write_str(tasks[i].syscall_mapping);
            omni_write_str("\n-------------------------------------------------\n");
        }
    }

    if (matches == 0) {
        omni_write_str("No system target matched the opcode.\n");
    }

    munmap(mapped_mem, sb.st_size);
    close(fd);
}

int main(int argc, char *argv[]) {
    unsigned int opcode = (argc > 1) ? (unsigned int)(argv[1][0] - '0') : 0;
    omni_execute_sys_diag("category8_sys.obx", opcode);
    return 0;
}
