#include "global.h"

#include "overlay_89.h"

void *ov89_02259E48(void *);

void ov89_0225A148(void *, s32);

BOOL ov89_0225C8F0(s32, void *);

// the three callbacks of the ov89_0225D64C table (UnkFuncPtr_ov66_022589B4: id, data, size, work)
void ov89_0225C93C(s32 param_1, const void *param_2, u32 param_3, void *param_4) {
    void *pvVar1;
    BOOL iVar2;

    pvVar1 = ov89_02259E48(param_4);
    iVar2 = ov89_0225C8F0(param_1, pvVar1);
    if (iVar2 != 0) {
        ov89_0225A148(param_4, param_1);
    }
    return;
}

u32 ov45_0222EC7C(s32);

void ov89_0225A160(void *, u32);

void ov89_0225C960(s32 param_1, const void *param_2, u32 param_3, void *param_4) {
    void *pvVar1;
    u32 uVar1;
    BOOL iVar2;

    pvVar1 = ov89_02259E48(param_4);
    iVar2 = ov89_0225C8F0(param_1, pvVar1);
    if (iVar2 != 0) {
        uVar1 = ov45_0222EC7C(param_1);
        ov89_0225A160(param_4, uVar1);
    }
    return;
}

void ov89_02259EC4(void *, const void *, s32);

void ov89_0225C988(s32 param_1, const void *param_2, u32 param_3, void *param_4) {
    void *pvVar1;
    BOOL iVar2;

    pvVar1 = ov89_02259E48(param_4);
    iVar2 = ov89_0225C8F0(param_1, pvVar1);
    if (iVar2 != 0) {
        ov89_02259EC4(param_4, param_2, param_1);
    }
    return;
}
