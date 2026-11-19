#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_gfx_loader.h"
#include "options.h"
#include "overlay_47.h"
#include "pm_string.h"
#include "save.h"
#include "text.h"
#include "unk_0200A090.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "msgdata.h"

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct UnkStruct_ov66_0222DFF8_t;

typedef u32 TextColor;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[6];
    u16 unk_60;
    u16 unk_62;
    s32 unk_64;
    s32 unk_68[3];
    s32 unk_74[3];
    void *unk_80;
    NNSG2dCharacterData *unk_84;
    UnkStruct_ov68_0225DB8C unk_88;
    UnkStruct_ov68_0225DB8C unk_A8;
    UnkStruct_ov68_0225DC74 unk_C8;
    UnkStruct_ov68_0225DC74 unk_DC;
} UnkStruct_ov68_0225D388;

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

String *ov47_02259D58(const UnkStruct_ov68_0225DB8C *param0, UnkStruct_ov68_0225CB70 *param1, u32 param2);

void ov47_022599F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_022599F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8) {
    String *v0;

    FillWindowPixelBuffer(&param0->unk_00[1], 0);
    FillWindowPixelBuffer(&param0->unk_00[5], 0);

    v0 = ov47_02259D58(param1, param2, 0);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[1], FONT_MESSAGE, v0, 0, 0, 0xFF, param6, 0L);

    v0 = ov47_02259D58(param1, param2, 1);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[1], FONT_MESSAGE, v0, 0, 24, 0xFF, param6, 0L);

    v0 = ov47_02259D58(param1, param2, 2);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[1], FONT_MESSAGE, v0, 0, 48, 0xFF, param6, 0L);

    if ((param4->questionNumber == param1->unk_04) && (param4->selectedAnswer < 3)) {
        s32 v1;
        s32 v2;
        TextColor v3;

        if (param8 == 1) {
            v3 = param7;
        } else {
            v3 = param6;
        }

        ov47_02258D78(param2, 0, heapID);

        v0 = ov47_02258CEC(param2, 0, 110);
        v1 = FontID_String_GetWidth(FONT_MESSAGE, v0, 0);
        v2 = (((12 * 8) - v1) / 2);

        AddTextPrinterParameterizedWithColor(&param0->unk_00[5], FONT_MESSAGE, v0, v2, 0, 0xFF, v3, 0L);

        v0 = ov47_02259D58(param1, param2, param4->selectedAnswer);
        v1 = FontID_String_GetWidth(FONT_MESSAGE, v0, 0);
        v2 = (((13 * 8) - v1) / 2);

        AddTextPrinterParameterizedWithColor(&param0->unk_00[5], FONT_MESSAGE, v0, (12 * 8) + v2, 0, 0xFF, param6, 0L);
    }

    ScheduleWindowCopyToVram(&param0->unk_00[5]);
    ScheduleWindowCopyToVram(&param0->unk_00[1]);
}

u32 ov47_02259DA4(const UnkStruct_ov68_0225DC74 *param0, u32 param1, u32 param2);

void ov47_02259B30(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DC74 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3);

void ov47_02259B30(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DC74 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3) {
    int v0;

    param0->unk_64 = 0;

    for (v0 = 0; v0 < 3; v0++) {
        param0->unk_68[v0] = 0;
        param0->unk_74[v0] = ov47_02259DA4(param1, v0, 80);

        SetWindowPaletteNum(&param0->unk_00[2 + v0], param3);
        FillWindowPixelBuffer(&param0->unk_00[2 + v0], 0);
    }
}

BOOL ov47_02259B74(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225C91C *param1);

BOOL ov47_02259B74(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225C91C *param1) {
    s32 v0;
    s32 v1;
    s32 v2;
    int v3, v4;
    int v5;

    if (param0->unk_64 >= 20) {
        return 1;
    }

    param0->unk_64++;

    v0 = (param0->unk_64 * 80) / 20;
    v5 = 0;

    for (v3 = 0; v3 < 3; v3++) {
        if (param0->unk_68[v3] == param0->unk_74[v3]) {
            v5++;
            continue;
        }

        if (param0->unk_74[v3] >= v0) {
            v2 = v0 - param0->unk_68[v3];
            v1 = param0->unk_68[v3];
            param0->unk_68[v3] = v0;
        } else {
            v2 = param0->unk_74[v3] - param0->unk_68[v3];
            v1 = param0->unk_68[v3];
            param0->unk_68[v3] = param0->unk_74[v3];
        }

        for (v4 = 0; v4 < v2; v4++) {
            BlitBitmapRectToWindow(&param0->unk_00[2 + v3], param0->unk_84->pRawData, 247, 16, 256, 32, 4 + v1 + v4, 0, 1, 16);
        }

        ScheduleWindowCopyToVram(&param0->unk_00[2 + v3]);
    }

    if (v5 == 3) {
        return 1;
    }

    return 0;
}

void ov47_0225999C(Window *, u32, u32, u32);

void ov47_02259C3C(Window *param_1, u32 param_2, void **param_3, enum HeapID param_4) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    GfGfxLoader_LoadScrnDataFromOpenNarc((NARC *)param_3[0x50], 0xcb, (BgConfig *)*param_3, GF_BG_LYR_MAIN_0, 0, 0, 0, param_4);
    window = param_1;
    do {
        FillWindowPixelBuffer(window, 0);
        CopyWindowToVram(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 6);
    ov47_0225999C(param_1, param_2, 0x77, 0x50600);
    return;
}
