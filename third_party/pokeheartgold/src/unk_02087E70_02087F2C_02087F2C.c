#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_40.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "unk_02087E70.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct SysTask;

struct SysTaskQueue;

struct System;

void *sub_02087A78(OverlayManager *man);

int sub_02087988(void *mgr);

void ov40_0222B934(void *a0);

void ov40_0222CF94(void *a0);

void ov40_0222D55C(void *a0);

FS_EXTERN_OVERLAY(OVY_40);

BOOL sub_02087F2C(OverlayManager *man, int *state);

BOOL sub_02087F2C(OverlayManager *man, int *state) {
    void *ctx = sub_02087A78(man);
    switch (*state) {
    case 0:
        ov40_0222CABC(ctx);
        (*state)++;
        break;
    case 1:
        ov40_0222CA8C(ctx);
        ov40_0222CF94(ctx);
        ov40_0222D55C(ctx);
        (*state)++;
        break;
    case 2:
        if (sub_02087988(*(void **)((u8 *)(ctx) + 0x6F0)) == 0) {
            (*state)++;
        }
        break;
    case 3:
        if (sub_02087988(*(void **)((u8 *)(ctx) + 0x6F4)) == 0) {
            (*state)++;
        }
        break;
    default:
        ov40_0222B934(ctx);
        Heap_Destroy(HEAP_ID_109);
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_40));
        gSystem.screensFlipped = 0;
        GfGfx_SwapDisplay();
        Field_SetEnvironmentSoundState_None_Unk2();
        return 1;
    }
    return 0;
}
