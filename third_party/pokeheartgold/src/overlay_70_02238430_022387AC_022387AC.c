#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

typedef struct Ov70Sys {
    u8 filler0[4];
    BgConfig *bgConfig;
    u8 filler8[0x1204 - 8];
    void (*cb1)(struct Ov70Sys *);
    void (*cb2)(struct Ov70Sys *);
} Ov70Sys;

void ov70_022387AC(Ov70Sys *sys);

void ov70_022387AC(Ov70Sys *sys) {
    void (*cb)(Ov70Sys *) = sys->cb1;
    if (cb != NULL) {
        cb(sys);
        sys->cb1 = NULL;
    }
    if (sys->cb2 != NULL) {
        sys->cb2(sys);
    }
    DoScheduledBgGpuUpdates(sys->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(u32 *)HW_INTR_CHECK_BUF |= 1;
#else
    {
        u8 *base = (u8 *)0x027E0000;
        *(u32 *)(base + 0x3FF8) |= 1;
    }
#endif
}
