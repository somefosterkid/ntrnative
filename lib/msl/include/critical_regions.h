// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/critical_regions.h, with
// Black's names of NitroSDK's mutex functions (nitro/os.h: OS_TryLockMutex, OS_LockMutex and OS_UnlockMutex)
#ifndef MSL_COMMON_CRITICAL_REGIONS_H
#define MSL_COMMON_CRITICAL_REGIONS_H

#include <nitro/os.h>

enum critical_regions {
    atexit_funcs_access   = 0,
    malloc_pool_access    = 1,
    stdin_access          = 2,
    stdout_access         = 3,
    stderr_access         = 4,
    files_access          = 5,
    console_status_access = 6,
    signal_funcs_access   = 7,
    thread_access         = 8,
    num_critical_regions  = 9
};

extern OSMutex __cs[num_critical_regions];
extern int __cs_id[num_critical_regions];
extern int __cs_ref[num_critical_regions];

static inline void __begin_critical_region(int region) {
    OSThread *currentThread;

    if (OS_TryLockMutex(&__cs[region]) == 0) {
        currentThread    = OS_GetCurrentThread();
        __cs_id[region]  = OS_GetThreadId(currentThread);
        __cs_ref[region] = 1;
    } else {
        currentThread = OS_GetCurrentThread();
        if (OS_GetThreadId(currentThread) == __cs_id[region]) {
            __cs_ref[region]++;
        } else {
            OS_LockMutex(&__cs[region]);
            currentThread    = OS_GetCurrentThread();
            __cs_id[region]  = OS_GetThreadId(currentThread);
            __cs_ref[region] = 1;
        }
    }
}

static inline void __end_critical_region(int region) {
    if (--__cs_ref[region] == 0) {
        OS_UnlockMutex(&__cs[region]);
    }
}

#endif // MSL_COMMON_CRITICAL_REGIONS_H
