#include "global.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct GraphicsModes;

struct MessageBank;

struct MessageBankEntry;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchScreenHitTableActionStateInternal;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

void NintendoWFC_StartVoiceChat(enum HeapID heapID);

void ov00_021E6A4C(void);

BOOL sub_02037030(int cmd, const void *data, int size);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void sub_0203A914(void);

void NetworkIcon_CreateOnSubScreen(BOOL isUpperScreen, u32 heapID);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

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

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

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

void ov114_0225D180(UnkStruct_ov114_0225D084 *param0, u32 param1);

void ov90_02259464(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, u32 param3);

BOOL ov90_02259538(const UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_02259554(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

BOOL ov90_0225B38C(UnkStruct_ov114_0225F270 *param0, UnkStruct_ov114_0225D338 *param1, UnkStruct_ov114_0225D084 *param2, u32 param3, u32 heapID);

void sub_0203A948(BOOL isUpperScreen, u32 heapID);

void ov00_021E69A8(s32 arg0);

void ov90_0225927C(UnkStruct_ov114_0225D084 *param0, u32 param1);

BOOL ov90_0225B38C(UnkStruct_ov114_0225F270 *param0, UnkStruct_ov114_0225D338 *param1, UnkStruct_ov114_0225D084 *param2, u32 param3, u32 heapID) {
    u32 v0;
    switch (param0->unk_00) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_IN, ((0 << 10) | (0 << 5)) | 0, 6, 1, (enum HeapID)heapID);
        sub_0203A948(0, heapID);
        if (param0->unk_01) {
            ov00_021E69A8((enum HeapID)heapID);
        }
        param0->unk_00++;
        break;

    case 1:
        if (IsPaletteFadeFinished()) {
            param0->unk_00++;
        }
        break;

    case 2:
        ov90_0225927C(param2, param3);
        ov90_02259464(param1, param2, 3, 1);
        param0->unk_00++;
        break;

    case 3:
        v0 = ov90_02259538(param1, 1);
        if (v0 == 1) {
            param0->unk_00++;
        }
        break;

    case 4:
        YesNoPrompt_InitFromTemplate(param0->unk_04, &param0->unk_08);
        param0->unk_00++;
        break;

    case 5:
        v0 = YesNoPrompt_HandleInput(param0->unk_04);
        if ((v0 == 1) || (v0 == 2)) {
            if (v0 == 1) {
                sub_02037030(22, 0L, 0);
            } else if (v0 == 2) {
                sub_02037030(23, 0L, 0);
            }
            ov90_02259464(param1, param2, 15, 1);
            ov90_02259554(param1, 1);
            YesNoPrompt_Reset(param0->unk_04);
            param0->unk_00++;
        }
        break;

    case 6:
        if (param0->unk_1E) {
            if (param0->unk_1F) {
                param0->unk_00 = 10;
            } else {
                param0->unk_00++;
            }
        }
        break;

    case 7:
        ov90_02259464(param1, param2, 6, 1);
        param0->unk_00++;
        break;

    case 8:
        v0 = ov90_02259538(param1, 1);
        if (v0 == 1) {
            param0->unk_00++;
            param0->unk_02 = 102;
        }
        break;

    case 9:
        param0->unk_02--;
        if (param0->unk_02 == 0) {
            param0->unk_00++;
        }
        break;

    case 10:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, ((0 << 10) | (0 << 5)) | 0, 6, 1, (enum HeapID)heapID);
        param0->unk_00++;
        break;

    case 11:
        if (IsPaletteFadeFinished()) {
            sub_0203A914();
            if (param0->unk_01) {
                ov00_021E6A4C();
            }
            param0->unk_00++;
        }
        break;

    case 12:
        return 1;
    }

    return 0;
}
