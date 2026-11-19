#include "global.h"

#include "error_handling.h"
#include "overlay_120.h"


extern u8 ov120_022602E4[];
extern u8 ov120_022602FC[];
extern u8 ov120_02260314[];
extern u8 ov120_0226032C[];

#if defined(SDK_PORT)
// On PC the overlay's asm rodata (asm/overlay_120.s) is not linked; these are byte-for-byte copies of it.
u8 ov120_022602E4[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 ov120_022602FC[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 ov120_02260314[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 ov120_0226032C[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
#endif

BOOL ov120_0225F318(Ov120Tiles *work) {
    int idx;
    int i;
    u32 res;
    u32 res2;

    GF_ASSERT(work != NULL);
    if (work->active == FALSE) {
        return TRUE;
    }
    if (work->count < 24) {
        work->timer--;
        if (work->timer <= 0) {
            work->timer = work->interval;
            idx = work->reverse ? ov120_0226032C[work->count] : ov120_022602FC[work->count];
            ov120_0225F040(work->tiles[idx], (idx % 8) * 32 + 16, (idx / 8) * 32 + 16, work->steps, work->window, 0, 32, 32, 32, work->color);
            idx = 47 - (work->reverse ? ov120_0226032C[work->count] : ov120_022602FC[work->count]);
            ov120_0225F040(work->tiles[idx], (idx % 8) * 32 + 16, (idx / 8) * 32 + 16, work->steps, work->window, 0, 32, 32, 32, work->color);
            work->count++;
        }
    }
    for (i = work->done; i < work->count; i++) {
        int idx2;
        if (work->reverse) {
            idx = ov120_0226032C[i];
            idx2 = 47 - ov120_0226032C[i];
        } else {
            idx = ov120_022602FC[i];
            idx2 = 47 - ov120_022602FC[i];
        }
        res = ov120_0225F08C(work->tiles[idx]);
        res2 = ov120_0225F08C(work->tiles[idx2]) | res;
        if (res2 == 1) {
            work->done++;
        }
    }
    if (work->done >= 24 && res2 == 1) {
        work->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

BOOL ov120_0225F4DC(Ov120Tiles *work) {
    int idx;
    int i;
    u32 res;
    u32 res2;

    GF_ASSERT(work != NULL);
    if (work->active == FALSE) {
        return TRUE;
    }
    if (work->count < 24) {
        work->timer--;
        if (work->timer <= 0) {
            work->timer = work->interval;
            if (work->reverse) {
                idx = ov120_02260314[work->count];
            } else {
                idx = 47 - ov120_02260314[work->count];
            }
            ov120_0225F040(work->tiles[idx], (idx % 8) * 32 + 16, (idx / 8) * 32 + 16, work->steps, work->window, 0, 0, 32, 32, work->color);
            if (work->reverse) {
                idx = ov120_022602E4[work->count];
            } else {
                idx = 47 - ov120_022602E4[work->count];
            }
            ov120_0225F040(work->tiles[idx], (idx % 8) * 32 + 16, (idx / 8) * 32 + 16, work->steps, work->window, 0, 0, 32, 32, work->color);
            work->count++;
        }
    }
    for (i = work->done; i < work->count; i++) {
        int idx2;
        if (work->reverse) {
            idx = ov120_02260314[i];
            idx2 = ov120_022602E4[i];
        } else {
            idx = 47 - ov120_02260314[i];
            idx2 = 47 - ov120_022602E4[i];
        }
        res = ov120_0225F08C(work->tiles[idx]);
        res2 = ov120_0225F08C(work->tiles[idx2]) | res;
        if (res2 == 1) {
            work->done++;
        }
    }
    if (work->done >= 24 && res2 == 1) {
        work->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_120_0225F120_0225F2B8_0225F318_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_022602E4", (void *)&ov120_022602E4, sizeof(ov120_022602E4), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_022602E4", (void *)&ov120_022602E4, sizeof(ov120_022602E4), "u8");
    PORT_AssetAdd("IPKE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_022602FC", (void *)&ov120_022602FC, sizeof(ov120_022602FC), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_022602FC", (void *)&ov120_022602FC, sizeof(ov120_022602FC), "u8");
    PORT_AssetAdd("IPKE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_02260314", (void *)&ov120_02260314, sizeof(ov120_02260314), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_02260314", (void *)&ov120_02260314, sizeof(ov120_02260314), "u8");
    PORT_AssetAdd("IPKE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_0226032C", (void *)&ov120_0226032C, sizeof(ov120_0226032C), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_120_0225F120_0225F2B8_0225F318.c:ov120_0226032C", (void *)&ov120_0226032C, sizeof(ov120_0226032C), "u8");
}
