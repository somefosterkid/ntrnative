#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "wild_encounter.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"

struct BgConfig;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct MessageBank;

struct MessageBankEntry;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchScreenHitTableActionStateInternal;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;

typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;

typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;

void CopyWindowToVram(Window *window);

void FillWindowPixelRect(Window *window, u8 color, u16 x, u16 y, u16 width, u16 height);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

u16 sub_0203769C(void);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

int ov37_021E75E8(UnkStruct_02095EAC *param0);

void ov37_021E7478(Window *param0, int param1, u32 param2, UnkStruct_02095EAC *param3);

void ov37_021E7478(Window *param0, int param1, u32 param2, UnkStruct_02095EAC *param3) {
    int v0, v1 = sub_0203769C();

    if (!ov37_021E75E8(param3)) {
        return;
    }

    for (v0 = 0; v0 < 5; v0++) {
        if (param3->unk_398[v0][0] == 0L) {
            Sprite_SetDrawFlag(param3->unk_274[v0], 0);
        } else {
            Sprite_SetDrawFlag(param3->unk_274[v0], 1);
        }
    }

    for (v0 = 0; v0 < 5; v0++) {
        FillWindowPixelRect(&param0[v0], 0, 0, 0, 10 * 8, 2 * 8);
    }

    for (v0 = 0; v0 < 5; v0++) {
        if (param3->unk_398[v0][0] != 0L) {
            PlayerName_FlatToString(param3->unk_398[v0][0], param3->unk_14[v0]);

            if (v1 == v0) {
                AddTextPrinterParameterizedWithColor(&param0[v0], FONT_MESSAGE, param3->unk_14[v0], 0, 0, 0xFF, ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
            } else {
                AddTextPrinterParameterizedWithColor(&param0[v0], FONT_MESSAGE, param3->unk_14[v0], 0, 0, 0xFF, param2, 0L);
            }
        }

        CopyWindowToVram(&param0[v0]);
    }
}
