#include "global.h"

#include "constants/heap.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_45_wifi_plaza_system.h"
#include "filesystem.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "screen_fade.h"
#include "screen_fade_internal.h"

struct BgConfig;

struct G2dRenderer;

struct Menu;

struct MessageLoader;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringTemplate;

struct SysTask;

struct SysTaskQueue;

struct System;

struct UnkStruct_ov66_0222DFF8_t;

struct WiFiPlazaProfile;

struct WiFiPlazaQuestion;

struct Window;

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

void ov45_0222AB0C(const WiFiPlazaProfile *profile, UnkStruct_ov66_0222E908 *param1);

BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef u32 TextColor;

typedef struct MessageLoader MessageLoader;

typedef struct Menu Menu;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;

extern System gSystem;

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

BOOL PlaySE(u16 seqID);

typedef struct StringTemplate StringTemplate;

typedef struct WiFiPlazaQuestion WiFiPlazaQuestion;

WiFiPlazaQuestion *Save_PhotoAlbum_Get(SaveData *saveData);

u32 WiFiPlazaQuestion_GetQuestionNumber(const WiFiPlazaQuestion *wiFiPlazaQuestion);

u32 WiFiPlazaQuestion_GetSelectedAnswer(const WiFiPlazaQuestion *wiFiPlazaQuestion);

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

typedef union {
    UnkStruct_ov68_0225D0F8 val1;
    UnkStruct_ov68_0225D388 val2;
} UnkUnion_ov68_0225CCA8;

typedef struct {
    MessageLoader *unk_00[9];
    StringTemplate *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2);

String *ov47_02259D40(const UnkStruct_ov68_0225DB8C *param0, UnkStruct_ov68_0225CB70 *param1);

void ov47_022599F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_02259B30(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DC74 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3);

BOOL ov47_02259B74(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225C91C *param1);

void ov47_02259C3C(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3);

void ov47_022592B4(UnkStruct_ov68_0225D128 *param0, const String *param1);

void ov47_02259318(UnkStruct_ov68_0225D128 *param0, const String *param1);

BOOL ov47_022593B4(const UnkStruct_ov68_0225D128 *param0);

void ov47_022593CC(UnkStruct_ov68_0225D128 *param0);

void ov47_02259404(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, u32 param2);

u32 ov47_02259430(UnkStruct_ov68_0225D128 *param0, u32 param1);

void ov47_02258A1C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov47_02258AA0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);

BOOL ov68_0225D36C(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259488(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);

void ov47_02259C8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

BOOL ov47_02259578(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

WiFiPlazaQuestion *sub_020318E8(SaveData *saveData);

u32 sub_020318F4(const WiFiPlazaQuestion *wiFiPlazaQuestion);

u32 sub_020318F8(const WiFiPlazaQuestion *wiFiPlazaQuestion);

BOOL ov47_02259578(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID) {
    String *v0;
    u32 v1;

    switch (param0->unk_60) {
    case 0:
        if (param0->unk_88.unk_08 == 0) {
            v1 = 103;
        } else {
            v1 = 105;
        }

        v0 = ov47_02258CEC(param1, 0, v1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 1;
        param0->unk_60 = 22;
        break;
    case 1:
        v0 = ov47_02259D40(&param0->unk_88, param1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 2;
        param0->unk_60 = 22;
        break;
    case 2:
        if (gSystem.newKeys & 0x0001) {
            v0 = ov47_02258CEC(param1, 0, 104);
            ov47_022592B4(param3, v0);
            param0->unk_62 = 3;
            param0->unk_60 = 22;
        }
        break;
    case 3:
        if (param0->unk_88.unk_08 == 0) {
            v1 = 107;
        } else {
            v1 = 108;
        }

        v0 = ov47_02258CEC(param1, 0, v1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 4;
        param0->unk_60 = 22;
        break;
    case 4: {
        UnkStruct_ov66_0222E908 v2;
        const WiFiPlazaProfile *v3;
        BOOL v4;

        v3 = ov45_0222A5C0(param4->unk_04);
        ov45_0222AB0C(v3, &v2);

        v4 = ov45_0222AB28(param4->unk_04, ov45_0222A53C(param4->unk_04));
        ov47_022599F0(param0, &param0->unk_88, param1, param2, &v2, heapID, ((TextColor)(((9 & 0xFF) << 16) | ((10 & 0xFF) << 8) | ((0 & 0xFF) << 0))), ((TextColor)(((13 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), v4);
        ov47_02259B30(param0, &param0->unk_C8, param2, 1);

        PlaySE(1495);
    }

        param0->unk_60 = 5;
        break;
    case 5: {
        BOOL v5;

        v5 = ov47_02259B74(param0, param2);

        if (v5 == 1) {
            param0->unk_60 = 6;
        }
    } break;
    case 6:
        if (gSystem.newKeys & 0x0001) {
            if (param0->unk_A8.unk_00 != (0xFFFFFFFF)) {
                param0->unk_60 = 7;
            } else {
                v0 = ov47_02258CEC(param1, 0, 112);
                ov47_022592B4(param3, v0);
                param0->unk_62 = 20;
                param0->unk_60 = 22;
            }
        }
        break;
    case 7:
        v0 = ov47_02258CEC(param1, 0, 111);
        ov47_02259318(param3, v0);
        ov47_02259404(param3, param2, heapID);
        param0->unk_60 = 8;
        break;
    case 8: {
        u32 v6;

        v6 = ov47_02259430(param3, heapID);

        switch (v6) {
        case 0:
            param0->unk_60 = 9;
            break;
        case 0xfffffffe:
            param0->unk_60 = 20;
            break;
        }
    } break;
    case 9:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, heapID);
        param0->unk_60 = 10;
        break;
    case 10:
        if (IsPaletteFadeFinished() == 1) {
            ov47_02259C3C(param0, param1, param2, heapID);
            param0->unk_60 = 11;
        }
        break;
    case 11:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, heapID);
        param0->unk_60 = 12;
        break;
    case 12:
        if (IsPaletteFadeFinished() == 1) {
            param0->unk_60 = 13;
        }
        break;
    case 13:
        if (param0->unk_A8.unk_08 == 0) {
            v1 = 113;
        } else {
            v1 = 115;
        }

        v0 = ov47_02258CEC(param1, 0, v1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 14;
        param0->unk_60 = 22;
        break;
    case 14:
        v0 = ov47_02259D40(&param0->unk_A8, param1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 15;
        param0->unk_60 = 22;
        break;
    case 15:
        if (gSystem.newKeys & 0x0001) {
            v0 = ov47_02258CEC(param1, 0, 114);
            ov47_022592B4(param3, v0);
            param0->unk_62 = 16;
            param0->unk_60 = 22;
        }
        break;
    case 16:
        if (param0->unk_A8.unk_08 == 0) {
            v1 = 117;
        } else {
            v1 = 118;
        }

        v0 = ov47_02258CEC(param1, 0, v1);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 17;
        param0->unk_60 = 22;
        break;
    case 17: {
        UnkStruct_ov66_0222E908 v7;
        WiFiPlazaQuestion *savedWiFiPlazaQuestion;
        BOOL v9;

        savedWiFiPlazaQuestion = sub_020318E8(param4->saveData);
        v7.questionNumber = sub_020318F4(savedWiFiPlazaQuestion);
        v7.selectedAnswer = sub_020318F8(savedWiFiPlazaQuestion);
        v9 = ov45_0222AB28(param4->unk_04, ov45_0222A53C(param4->unk_04));

        ov47_022599F0(param0, &param0->unk_A8, param1, param2, &v7, heapID, ((TextColor)(((11 & 0xFF) << 16) | ((12 & 0xFF) << 8) | ((0 & 0xFF) << 0))), ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), v9);
        ov47_02259B30(param0, &param0->unk_DC, param2, 2);
        PlaySE(1495);
    }
        param0->unk_60 = 18;
        break;
    case 18: {
        BOOL v10;

        v10 = ov47_02259B74(param0, param2);

        if (v10 == 1) {
            param0->unk_60 = 19;
        }
    } break;
    case 19:
        if (gSystem.newKeys & 0x0001) {
            param0->unk_60 = 20;
        }
        break;
    case 20:
        v0 = ov47_02258CEC(param1, 0, 120);
        ov47_022592B4(param3, v0);
        param0->unk_62 = 21;
        param0->unk_60 = 22;
        break;
    case 21:
        ov47_022593CC(param3);
        return 1;
    case 22:
        if (ov47_022593B4(param3) == 1) {
            param0->unk_60 = param0->unk_62;
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}
