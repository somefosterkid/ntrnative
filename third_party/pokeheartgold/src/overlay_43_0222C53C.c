#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "text.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"
#include "sprite.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void ov43_0222C53C(void *arg0, struct PalPadShared *arg1, s16 arg2);

struct Menu;
struct SpriteList;
struct ListMenuItem;
struct UnkStruct_ov63_0222AE60_t;
struct UnkStruct_ov63_0222BB38_t;
typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;
typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;
typedef struct Menu Menu;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;
typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    Window unk_08;
    UnkStruct_ov63_0222BB38 *unk_18[8];
} UnkStruct_ov64_02230074;
void ov43_0222C600(PalPadRoster *param0);

s32 ov43_0222C620(void *arg0);

void ov43_0222C53C(void *arg0, PalPadShared *arg1, s16 arg2) {
    *((s8 *)&arg1->subState) = 3;
    *((s16 *)&((PalPadRoster *)arg0)->scrollDir) = arg2;
    ((PalPadRoster *)arg0)->scrollStep = 0;
}

void ov43_0222C550(void *arg0, struct PalPadGfx *arg1, s32 arg2, enum HeapID arg3);

void ov43_0222C550(void *arg0, PalPadGfx *arg1, s32 strno, enum HeapID heapID) {
    String *string;

    FillWindowPixelBuffer(&((PalPadRoster *)arg0)->msgWindow, 0xFU);
    string = String_New(0x80U, heapID);
    ReadMsgDataIntoString(*((MsgData * *)&arg1->msgData), strno, string);
    StringExpandPlaceholders(*((MessageFormat * *)&arg1->msgFormat), ((PalPadRoster *)arg0)->msgText, string);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    *((s32 *)&((PalPadRoster *)arg0)->printerId) = (s32)AddTextPrinterParameterizedWithColor(&((PalPadRoster *)arg0)->msgWindow, FONT_MESSAGE, ((PalPadRoster *)arg0)->msgText, 0U, 0U, ((PalPadRoster *)arg0)->textSpeed, 0x10200U, NULL);
    DrawFrameAndWindow2(&((PalPadRoster *)arg0)->msgWindow, 1, 0x46U, 0xCU);
    ScheduleWindowCopyToVram(&((PalPadRoster *)arg0)->msgWindow);
    String_Delete(string);
}

BOOL ov43_0222C5D8(const PalPadRoster *arg0, PalPadShared *arg1) {
    if (TextPrinterCheckActive((u8)arg0->printerId) == 0) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        arg1->inputState = TextFlags_GetIsTouchSpeedingUpPrint();
        return 1;
    }
    return 0;
}

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov43_0222C600(PalPadRoster *param0) {
    ClearFrameAndWindow2(&param0->msgWindow, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->msgWindow);
    ScheduleWindowCopyToVram(&param0->returnWindow);
}

s32 ov43_0222C620(void *arg0) {
    int new_var;
    new_var = 4;
    return ((*((s16 *)arg0)) * 8) + (*((s16 *)(((u8 *)arg0) + new_var)));
}
