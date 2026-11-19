#include "global.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "font.h"

struct GraphicsModes;

struct String;

struct Window;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

extern void FontID_Alloc(enum Font font, enum HeapID heapID);

extern void FontID_Release(enum Font font);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

typedef struct {
    Window unk_00;
    u32 unk_10;
    String *unk_14;
    UnkStruct_ov114_0225CBF0 unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

extern void ov90_02258CF0(UnkStruct_ov114_0225CBF0 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

void ov90_0225A5BC(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 param2);

extern const TextColor ov90_0225C210[UnkEnum_ov66_022324D0_12];

extern const fx32 ov90_0225C258[UnkEnum_ov66_022324D0_12];

extern const fx32 ov90_0225C228[UnkEnum_ov66_022324D0_12];

extern const fx32 ov90_0225C21C[UnkEnum_ov66_022324D0_12];

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_0225A350(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, u32 heapID);

void ov90_0225A350(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, u32 heapID) {
    u32 v0;
    u32 v1;

    param0->unk_2C = 0;
    param0->unk_2E = 0;

    ov90_02258CF0(&param0->unk_18, ov90_0225C258[param0->unk_10], ov90_0225C228[param0->unk_10], ov90_0225C21C[param0->unk_10], 16);
    ov90_0225A5BC(param0, param1, param0->unk_18.unk_00 >> 12);
    FontID_Alloc(FONT_SUBSCREEN, (enum HeapID)(heapID));

    v0 = FontID_String_GetWidth(FONT_SUBSCREEN, param0->unk_14, 0);
    v1 = ((20 * 8) / 2) - (v0 / 2);

    AddTextPrinterParameterizedWithColor(&param0->unk_00, FONT_SUBSCREEN, param0->unk_14, v1, 0, 0, ov90_0225C210[param0->unk_10], 0L);
    FontID_Release(FONT_SUBSCREEN);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
}
