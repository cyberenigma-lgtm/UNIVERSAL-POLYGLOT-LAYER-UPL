/**
 * RoR Memory Guard (Proof of Concept)
 * 
 * Simple Win32 background watchdog that monitors WAR.exe / WAR-64.exe
 * and triggers K32EmptyWorkingSet when private working set exceeds a threshold,
 * releasing unreferenced cache pages to mitigate 32-bit heap exhaustion in high-density RvR.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_THRESHOLD_MB 2200
#define POLL_INTERVAL_MS     3000

static HANDLE find_process_by_name(const char *process_name, DWORD *out_pid) {
    DWORD pids[1024], bytes_needed;
    if (!EnumProcesses(pids, sizeof(pids), &bytes_needed)) {
        return NULL;
    }

    unsigned int count = bytes_needed / sizeof(DWORD);
    for (unsigned int i = 0; i < count; i++) {
        if (pids[i] == 0) continue;

        HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_SET_QUOTA, FALSE, pids[i]);
        if (h) {
            char name[MAX_PATH] = "";
            HMODULE h_mod;
            DWORD mod_needed;
            if (EnumProcessModules(h, &h_mod, sizeof(h_mod), &mod_needed)) {
                GetModuleBaseNameA(h, h_mod, name, sizeof(name));
                if (_stricmp(name, process_name) == 0) {
                    *out_pid = pids[i];
                    return h;
                }
            }
            CloseHandle(h);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    size_t threshold_mb = DEFAULT_THRESHOLD_MB;
    if (argc > 1) {
        int parsed = atoi(argv[1]);
        if (parsed > 500) threshold_mb = (size_t)parsed;
    }

    printf("====================================================================\n");
    printf("   WARHAMMER ONLINE (ROR) - WIN32 WORKING SET WATCHDOG (PoC)       \n");
    printf("====================================================================\n");
    printf("[INFO] Threshold set to: %zu MB\n", threshold_mb);
    printf("[INFO] Polling interval: %d ms\n", POLL_INTERVAL_MS);
    printf("[INFO] Waiting for WAR.exe or WAR-64.exe...\n\n");

    bool was_active = false;

    for (;;) {
        DWORD pid32 = 0, pid64 = 0;
        HANDLE h32 = find_process_by_name("WAR.exe", &pid32);
        HANDLE h64 = find_process_by_name("WAR-64.exe", &pid64);

        HANDLE h_target = h64 ? h64 : h32;
        const char *target_name = h64 ? "WAR-64.exe" : (h32 ? "WAR.exe" : NULL);
        DWORD target_pid = h64 ? pid64 : pid32;

        if (h32 && h64) {
            CloseHandle(h32); // Discard redundant handle
        }

        if (h_target) {
            was_active = true;
            PROCESS_MEMORY_COUNTERS_EX pmc;
            if (GetProcessMemoryInfo(h_target, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                size_t ws_mb = pmc.WorkingSetSize / (1024 * 1024);
                size_t commit_mb = pmc.PrivateUsage / (1024 * 1024);

                if (ws_mb > threshold_mb) {
                    printf("\n[ALERT] %s (PID: %lu) exceeded threshold: %zu MB (Commit: %zu MB)\n",
                           target_name, target_pid, ws_mb, commit_mb);
                    printf("[ACTION] Invoking K32EmptyWorkingSet()...\n");

                    if (EmptyWorkingSet(h_target)) {
                        PROCESS_MEMORY_COUNTERS_EX post;
                        GetProcessMemoryInfo(h_target, (PROCESS_MEMORY_COUNTERS*)&post, sizeof(post));
                        size_t post_ws = post.WorkingSetSize / (1024 * 1024);
                        size_t freed = (ws_mb > post_ws) ? (ws_mb - post_ws) : 0;
                        printf("[SUCCESS] Working set reduced: %zu MB -> %zu MB (Freed: %zu MB)\n\n",
                               ws_mb, post_ws, freed);
                    }
                } else {
                    printf("\r[STATUS] %s active | Working Set: %4zu MB | Commit: %4zu MB | OK   ",
                           target_name, ws_mb, commit_mb);
                    fflush(stdout);
                }
            }
            CloseHandle(h_target);
        } else {
            if (was_active) {
                printf("\n[INFO] Client closed. Waiting for new session...\n");
                was_active = false;
            } else {
                printf("\r[STATUS] Waiting for WAR.exe / WAR-64.exe...                        ");
                fflush(stdout);
            }
        }

        Sleep(POLL_INTERVAL_MS);
    }

    return 0;
}
