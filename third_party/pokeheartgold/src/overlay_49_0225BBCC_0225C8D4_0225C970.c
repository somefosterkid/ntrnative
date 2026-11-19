#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

u32 ov45_0222AAC8(const WiFiPlazaProfile *);

String *ov49_0225B388(UnkStruct_ov70_0225F208 *, s32, s32);

void ov49_0225BB14(UnkStruct_ov70_0225E4EC *, NARC *, u32, u32, u32, enum HeapID);

void ov49_0225CAD4(UnkStruct_ov70_02260744 *param_1, UnkStruct_ov70_0225E4EC *param_2, u32 param_3, NARC *param_4, u32 param_5);

void ov49_0225C970(UnkStruct_ov70_02260744 *param_1, UnkStruct_ov70_0225E4EC *param_2, UnkStruct_ov70_0225F208 *param_3, const WiFiPlazaProfile *param_4, NARC *param_5, enum HeapID param_6);

void ov49_0225C970(UnkStruct_ov70_02260744 *param_1, UnkStruct_ov70_0225E4EC *param_2, UnkStruct_ov70_0225F208 *param_3, const WiFiPlazaProfile *param_4, NARC *param_5, enum HeapID param_6) {
    u16 uVar1;
    String *string;

    GfGfxLoader_LoadScrnDataFromOpenNarc(param_5, 0x37, param_2->unk_00, GF_BG_LYR_MAIN_CNT, 0, 0, 0, param_6);
    param_1->unk_08 = 0;
    uVar1 = ov45_0222AAC8(param_4);
    param_1->unk_0A = uVar1;
    GfGfxLoader_LoadCharDataFromOpenNarc(param_5, (u32)param_1->unk_0A, param_2->unk_00, GF_BG_LYR_SUB_2, 0x140, 0, 0, param_6);
    ov49_0225BB14(param_2, param_5, param_1->unk_0A + 0x1b, 6, 0x140, param_6);
    string = ov49_0225B388(param_3, 1, 0x3f);
    FillWindowPixelBuffer(&param_1->unk_0C, 0);
    AddTextPrinterParameterizedWithColor(&param_1->unk_0C, FONT_SYSTEM, string, 0, 4, 0, 0xf0e00, (PrinterCallback_t)0x0);
    if (param_1->unk_02 == 0) {
        ov49_0225CAD4(param_1, param_2, 0, param_5, param_6);
        return;
    }
    ov49_0225CAD4(param_1, param_2, 2, param_5, param_6);
    return;
}
