#include "global.h"

#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"

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

void ov90_0225BD84(UnkStruct_ov114_0225FAB8 *param0);

extern const VecFx32 ov90_0225C2B4[2];

void ov90_0225BC28(UnkStruct_ov114_0225FAB8 *param0, s32 param1);

void ov90_0225BD08(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225BC28(UnkStruct_ov114_0225FAB8 *param0, s32 param1) {
    VecFx32 v0;
    int v1;
    int v2;

    param0->unk_0A = (param0->unk_08 * 23) / 8;

    if (((param0->unk_08 + param1) <= 8) && ((param0->unk_08 + param1) >= 0)) {
        v2 = ((param0->unk_08 + param1) * 23) / 8;
    } else {
        v2 = param0->unk_0A;
    }

    for (v1 = 0; v1 < 2; v1++) {
        v0 = ov90_0225C2B4[v1];

        if (v1 == 0) {
            v0.y -= FX32_CONST(v2);
        } else {
            v0.y += FX32_CONST(v2);
        }

        Sprite_SetMatrix(param0->unk_00[v1], &v0);
    }

    ov90_0225BD84(param0);
}

void ov90_0225BD08(UnkStruct_ov114_0225FAB8 *param0) {
    int v0;

    if (param0->unk_18C) {
        SysTask_Destroy(param0->unk_18C);
        param0->unk_18C = NULL;
    }

    for (v0 = 0; v0 < 2; v0++) {
        if (param0->unk_00[v0] != NULL) {
            Sprite_Delete(param0->unk_00[v0]);
            param0->unk_00[v0] = NULL;
        }
    }

    HBlankInterruptDisable();

    GX_SetVisibleWnd(param0->unk_194);
    G2_SetWndOutsidePlane(param0->unk_190.planeMask, param0->unk_190.effect);
}
