#include "global.h"

#include "bg_window.h"
#include "font_types_def.h"
#include "gf_gfx_loader.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_75.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef u32 TextColor;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

int ov75_0224964C(Window *param0, String *param1, int param2, int param3, u32 param4, int param5);

void ov75_02249684(Window *param0, String *param1, int param2, int param3, int param4, TextColor param5);

void ov75_02249684(Window *param0, String *param1, int param2, int param3, int param4, TextColor param5) {
    param2 = ov75_0224964C(param0, param1, param2, param4, param5, 1);
    AddTextPrinterParameterizedWithColor(param0, FONT_MESSAGE, param1, param2, param3, 0, param5, 0L);
}

static void ov75_022496B8(UnkStruct_ov98_02247704 *arg0, s32 strno) {
    String *string;

    string = String_New(0x100U, HEAP_ID_116);
    ReadMsgDataIntoString(((MsgData *)arg0->unk_30), strno, string);
    StringExpandPlaceholders(((MessageFormat *)arg0->unk_20), arg0->unk_40, string);
    FillWindowPixelBuffer(&arg0->unk_68, 0xFU);
    DrawFrameAndWindow1(&arg0->unk_68, 1, 0x1FU, 0xBU);
    AddTextPrinterParameterized(&arg0->unk_68, FONT_MESSAGE, arg0->unk_40, 0U, 0U, 0U, NULL);
    *((s32 *)&arg0->unk_44) = 0xFF;
    String_Delete(string);
}

void ov75_0224971C(UnkStruct_ov98_02247704 *arg0, s32 arg1, s32 arg2);

void ov75_0224971C(UnkStruct_ov98_02247704 *arg0, s32 arg1, s32 num) {
    s32 v6;

    v6 = arg1;
    if (v6 == -1) {
        v6 = 0xB;
    }
    BufferIntegerAsString(((MessageFormat *)arg0->unk_20), 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ClearFrameAndWindow2(&arg0->unk_48, 1);
    ov75_022496B8(arg0, v6);
}

void ov75_02249758(ListMenu *param0, u32 param1, u8 param2) {
    if (param2 == 0) {
        PlaySE(1500);
    }
}

void ov75_0224976C(ListMenu *param0, u32 param1, u8 param2) {
    if (param2 == 0) {
        PlaySE(1500);
    }
}
