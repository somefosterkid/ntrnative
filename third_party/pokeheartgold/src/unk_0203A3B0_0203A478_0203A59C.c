#include "global.h"

#include "heap.h"
#include "network_icon.h"
#include "sys_task.h"

NNSG2dCharacterData *sub_0203A534(s32, s32, enum HeapID, void *);

void sub_0203A59C(s32 arg0, s32 arg1, enum HeapID arg2);

void sub_0203A59C(s32 arg0, s32 arg1, enum HeapID arg2) {
    s32 var_r6;
    s32 var_r4;
    NNSG2dCharacterData *temp_r5;
    void *sp4;
    s32 sp0;

    sp0 = arg1;
    if (arg0 == 1) {
        var_r4 = *(s32 *)REG_DISPCNT_ADDR & 0x300010;
        var_r6 = GX_GetBankForOBJ();
    } else {
        var_r4 = *(s32 *)REG_DB_DISPCNT_ADDR & 0x300010;
        var_r6 = GX_GetBankForSubOBJ();
    }
    if (var_r4 == 0x300010) {
        sp4 = Heap_AllocAtEnd(arg2, 0x4C0U);
    } else {
        sp4 = Heap_AllocAtEnd(arg2, 0x258U);
    }
    if (sp4 != NULL) {
        temp_r5 = sub_0203A534(sp0, var_r4, arg2, sp4);
        switch (var_r4) {
        case 0x10:
            if (var_r6 == GX_VRAM_OBJ_16_G || var_r6 == GX_VRAM_OBJ_16_F) {
                var_r6 = 0x3E00;
            } else {
                var_r6 = 0x7E00;
            }
            break;
        case 0x100010:
            if (var_r6 == GX_VRAM_OBJ_16_G || var_r6 == GX_VRAM_OBJ_16_F || var_r6 == 0x100) {
                var_r6 = 0x3E00;
            } else {
                var_r6 = 0xFE00;
            }
            break;
        case 0x200010:
            if (var_r6 == GX_VRAM_OBJ_80_EF || var_r6 == GX_VRAM_OBJ_80_EG) {
                var_r6 = 0x13E00;
            } else if (var_r6 == GX_VRAM_OBJ_64_E) {
                var_r6 = 0xFE00;
            } else {
                var_r6 = 0x1FE00;
            }
            break;
        case 0x300010:
            var_r6 = 0x3FC00;
            break;
        default:
            var_r6 = 0xFE00;
            break;
        }
        if (arg0 == 1) {
            if (var_r4 != 0x300010) {
                DC_FlushRange(temp_r5->pRawData, 0x200U);
                GX_LoadOBJ(temp_r5->pRawData, var_r6, 0x200U);
            } else {
                DC_FlushRange(temp_r5->pRawData, 0x400U);
                GX_LoadOBJ(temp_r5->pRawData, var_r6, 0x400U);
            }
        } else {
            if (var_r4 != 0x300010) {
                DC_FlushRange(temp_r5->pRawData, 0x200U);
                GXS_LoadOBJ(temp_r5->pRawData, var_r6, 0x200U);
            } else {
                DC_FlushRange(temp_r5->pRawData, 0x400U);
                GXS_LoadOBJ(temp_r5->pRawData, 0x3C00U, 0x400U);
            }
        }
        Heap_Free(sp4);
    }
}

struct NetworkIcon;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

int inline GetStrengthOAMOffset(enum NNS_G2D_VRAM_TYPE vramType, int strength);

NetworkIcon *CreateNetworkIcon(u32 unused0, u32 heapID, int x, int y, BOOL isWifi, void *unused, enum NNS_G2D_VRAM_TYPE vramType);

void sub_0203A700(NetworkIcon *icon);

void sub_0203A700(NetworkIcon *icon) {
    int strength;
    u32 sel;
    s32 mode;
    GXOamAttr *oam;
    int bank;
    int off;

    switch (icon->screenId) {
    case 1:
        sel = GX_GetDispSelect() == GX_DISP_SELECT_MAIN_SUB ? 1 : 2;
        break;
    case 2:
        sel = GX_GetDispSelect() == GX_DISP_SELECT_MAIN_SUB ? 2 : 1;
        break;
    default:
        sel = icon->vramType;
        break;
    }
    strength = icon->strength;
    if (sel == 1) {
        mode = reg_GX_DISPCNT & 0x00300010;
        bank = GX_GetBankForOBJ();
    } else {
        mode = reg_GXS_DB_DISPCNT & 0x00300010;
        bank = GX_GetBankForSubOBJ();
    }
    switch (mode) {
    case 0x10:
        if (bank == 0x40 || bank == 0x20) {
            off = 4 * strength + 496;
        } else {
            off = 4 * strength + 1008;
        }
        break;
    case 0x100010:
        if (bank == 0x40 || bank == 0x20 || bank == 0x100) {
            off = 2 * strength + 504;
        } else {
            goto def;
        }
        break;
    case 0x200010:
        if (bank == 0x30 || bank == 0x50) {
            off = strength + 636;
        } else if (bank == 0x10) {
            off = strength + 508;
        } else {
            off = strength + 1020;
        }
        break;
    case 0x300010:
        if (bank == 3) {
            off = strength + 1020;
        } else {
            off = strength + 508;
        }
        break;
    default:
def:
        off = 2 * strength + 1016;
        break;
    }
    oam = sel == 1 ? (GXOamAttr *)HW_OAM : (GXOamAttr *)HW_DB_OAM;
    G2_SetOBJAttr(oam, icon->x, icon->y, 0, (GXOamMode)icon->oamMode, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, off, icon->palette, 0);
    if (oam != icon->oam) {
        G2_SetOBJAttr(icon->oam, 0, 0, 0, (GXOamMode)icon->oamMode, FALSE, GX_OAM_EFFECT_NODISPLAY, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, 0, 0, 0);
        icon->oam = oam;
    }
}
