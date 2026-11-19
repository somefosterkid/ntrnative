#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "player_data.h"
#include "font.h"
#include "font_types_def.h"
#include "text.h"

struct SpriteList;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

void Sprite_SetMatrix(Sprite *sprite, const VecFx32 *position);

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[1];
    s32 unk_10;
    Sprite *unk_14;
    SpriteResource *unk_18[4];
    u16 unk_28;
    u16 unk_2A;
    UnkStruct_ov68_0225DB8C unk_2C;
} UnkStruct_ov68_0225D0F8;

extern const VecFx32 ov47_02259E78[3];

void ov47_022591F8(UnkStruct_ov68_0225D0F8 *param0);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);
void ov47_0225921C(UnkStruct_ov68_0225D0F8 *param0);

void ov47_02259228(void *arg0, BgConfig **arg1, SaveData *arg2, enum HeapID arg3);

struct Menu;
typedef struct Menu Menu;
typedef struct {
    u32 unk_00;
    s32 unk_04;
    Window unk_08;
    String *unk_18;
    void *unk_1C;
    Menu *unk_20;
} UnkStruct_ov68_0225D128;
void ov47_022593A0(UnkStruct_ov68_0225D128 *param0);
void ov47_02259448(UnkStruct_ov68_0225D128 *param0, u32 param1);
typedef u32 TextColor;
void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);
void *WaitingIcon_New(Window *window, u32 baseTile);
void ov47_02259278(UnkStruct_ov68_0225D128 *param0, u32 param1);
void ov47_022592B4(UnkStruct_ov68_0225D128 *param0, const String *param1);
void ov47_02259318(UnkStruct_ov68_0225D128 *param0, const String *param1);
void ov47_02259384(UnkStruct_ov68_0225D128 *param0);

void ov47_022591F8(UnkStruct_ov68_0225D0F8 *param0) {
    Sprite_SetDrawFlag(param0->unk_14, TRUE);
    Sprite_SetMatrix(param0->unk_14, &ov47_02259E78[param0->unk_10]);
}

void ov47_0225921C(UnkStruct_ov68_0225D0F8 *param0) {
    Sprite_SetAnimActiveFlag(param0->unk_14, 1);
}

void ov47_02259228(void *arg0, BgConfig **arg1, SaveData *saveData, enum HeapID heapID) {
    AddWindowParameterized(*arg1, arg0 + 8, 1U, 2U, 0x13U, 0x1BU, 4U, 4U, 0x28U);
    FillWindowPixelBuffer(arg0 + 8, 0xFU);
    ((UnkStruct_ov68_0225D128 *)arg0)->unk_18 = String_New(0x100U, heapID);
    *(s32 *)((u8 *)arg0 + 4) = (s32)Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
}

void ov47_02259278(UnkStruct_ov68_0225D128 *param0, u32 param1) {
    if (TextPrinterCheckActive(param0->unk_00)) {
        RemoveTextPrinter(param0->unk_00);
    }

    ov47_022593A0(param0);

    String_Delete(param0->unk_18);
    RemoveWindow(&param0->unk_08);

    ov47_02259448(param0, param1);
}

void ov47_022592B4(UnkStruct_ov68_0225D128 *param0, const String *param1) {
    if (TextPrinterCheckActive(param0->unk_00)) {
        RemoveTextPrinter(param0->unk_00);
    }

    FillWindowPixelBuffer(&param0->unk_08, 15);
    String_Copy(param0->unk_18, param1);
    param0->unk_00 = AddTextPrinterParameterizedWithColor(&param0->unk_08, FONT_MESSAGE, param0->unk_18, 0, 0, param0->unk_04, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
    DrawFrameAndWindow2(&param0->unk_08, 1, 1, 5);
}

void ov47_02259318(UnkStruct_ov68_0225D128 *param0, const String *param1) {
    if (TextPrinterCheckActive(param0->unk_00)) {
        RemoveTextPrinter(param0->unk_00);
    }

    FillWindowPixelBuffer(&param0->unk_08, 15);
    String_Copy(param0->unk_18, param1);
    AddTextPrinterParameterizedWithColor(&param0->unk_08, FONT_MESSAGE, param0->unk_18, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
    DrawFrameAndWindow2(&param0->unk_08, 1, 1, 5);
    ScheduleWindowCopyToVram(&param0->unk_08);
}

void ov47_02259384(UnkStruct_ov68_0225D128 *param0) {
    ((param0->unk_1C == 0L) ? (void)0 : GF_AssertFail());
    param0->unk_1C = WaitingIcon_New(&param0->unk_08, 1);
}
