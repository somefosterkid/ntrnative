#include "global.h"

#include "sys_task_api.h"
#include "overlay_90.h"
#include "sprite.h"

void ov90_0225BDE4(SysTask *task, void *data);

void ov90_0225BD84(void *arg0);

typedef struct {
    Sprite *unk_00[2];
    s16 unk_08;
    s16 unk_0A;
    u8 unk_0C[2][192];
    SysTask *unk_18C;
    GXWndPlane unk_190;
    GXWndPlane battlerType;
    int unk_194;
} UnkStruct_ov114_0225FAB8;
void ov90_0225BDE4(SysTask *param0, void *param1);

void ov90_0225BD84(void *arg0) {
    s32 v0;

    memset(arg0 + 0xCC, 0, 0xC0U);
    v0 = 0;
    if ((s32)(0x17 - ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_0A) > 0) {
        do {
            *(s8 *)((u8 *)(arg0 + v0) + 0x115) = 1;
            *(s8 *)((u8 *)(arg0 + (0x79 - v0)) + 0xCC) = 1;
            v0 += 1;
        } while (v0 < (s32)(0x17 - ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_0A));
    }
    if (((UnkStruct_ov114_0225FAB8 *)arg0)->unk_18C == NULL) {
        ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_18C = SysTask_CreateOnVWaitQueue(ov90_0225BDE4, arg0, 0U);
    }
}

void ov90_0225BDE4(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225FAB8 *v0 = param1;

    memcpy(v0->unk_0C[0], v0->unk_0C[1], 192);
    SysTask_Destroy(v0->unk_18C);

    v0->unk_18C = NULL;
}

void ov90_0225BE08(void *param0) {
    UnkStruct_ov114_0225FAB8 *v0 = param0;
    s32 v1;
    int v2;

    v1 = GX_GetVCount();
    v1 = (v1 + 1) % 256;

    if (v1 < 192) {
        v2 = GX_GetVisibleWnd();

        if (GX_IsHBlank()) {
            if ((v1 < (96 - 23)) || (v1 > ((96 + 2) + 23))) {
                G2_SetWndOutsidePlane(v0->unk_190.planeMask, v0->unk_190.effect);
                v2 |= v0->unk_194;
            } else {
                G2_SetWndOutsidePlane(v0->battlerType.planeMask, v0->battlerType.effect);
                v2 &= ~(GX_WNDMASK_W1 | v0->unk_194);
            }

            if (v0->unk_0C[0][v1] == 1) {
                GX_SetVisibleWnd(v2 | GX_WNDMASK_W0);
            } else {
                v2 &= ~GX_WNDMASK_W0;
                GX_SetVisibleWnd(v2);
            }
        }
    }
}
