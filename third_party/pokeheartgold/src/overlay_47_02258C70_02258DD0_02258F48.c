#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct BgConfig;

struct Menu;

struct MessageLoader;

struct SpriteList;

struct SpriteResourceCollection;

struct StringTemplate;

struct SysTaskQueue;

struct UnkStruct_ov66_0222DFF8_t;

typedef enum {
    UnkEnum_ov66_02233434_00,
    UnkEnum_ov66_02233434_01,
    UnkEnum_ov66_02233434_02,
    UnkEnum_ov66_02233434_03,
} UnkEnum_ov66_02233434;

void ov45_0222A7DC(WiFiPlazaSystem *param0, u32 param1, u32 param2);

void ov45_0222F4D4(s32 param0);

UnkEnum_ov66_02233434 ov45_0222F524(void);

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

typedef struct MessageLoader MessageLoader;

typedef struct Menu Menu;

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

typedef struct StringTemplate StringTemplate;

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
    MessageLoader *unk_00[9];
    StringTemplate *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2);

String *ov47_02259D40(const UnkStruct_ov68_0225DB8C *param0, UnkStruct_ov68_0225CB70 *param1);

void ov47_0225916C(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

void ov47_022591F8(UnkStruct_ov68_0225D0F8 *param0);

void ov47_0225921C(UnkStruct_ov68_0225D0F8 *param0);

void ov47_022592B4(UnkStruct_ov68_0225D128 *param0, const String *param1);

void ov47_02259318(UnkStruct_ov68_0225D128 *param0, const String *param1);

void ov47_02259384(UnkStruct_ov68_0225D128 *param0);

void ov47_022593A0(UnkStruct_ov68_0225D128 *param0);

BOOL ov47_022593B4(const UnkStruct_ov68_0225D128 *param0);

void ov47_022593CC(UnkStruct_ov68_0225D128 *param0);

void ov47_02259404(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, u32 param2);

u32 ov47_02259430(UnkStruct_ov68_0225D128 *param0, u32 param1, UnkStruct_ov68_0225C91C *);

BOOL ov47_02258F48(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, u32 param5);

BOOL ov47_02258F48(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, u32 param5) {
    u32 v0;
    String *v1;

    switch (param0->unk_28) {
    case 0:
        v1 = ov47_02258CEC(param1, 0, 94);
        ov47_022592B4(param3, v1);
        param0->unk_2A = 1;
        param0->unk_28 = 10;
        break;
    case 1:
        ov47_02259404(param3, param2, param5);
        param0->unk_28 = 2;
        break;
    case 2: {
        u32 v2;

        v2 = ov47_02259430(param3, param5, param2);

        switch (v2) {
        case 0:
            param0->unk_28 = 3;
            break;
        case 0xfffffffe:
            param0->unk_28 = 9;
            break;
        }
    } break;
    case 3:
        if (param0->unk_2C.unk_08 == 0) {
            v0 = 92;
        } else {
            v0 = 93;
        }

        v1 = ov47_02258CEC(param1, 0, v0);
        ov47_022592B4(param3, v1);

        param0->unk_2A = 4;
        param0->unk_28 = 10;
        break;
    case 4:
        ov47_0225916C(param0, param1, param2);

        v1 = ov47_02259D40(&param0->unk_2C, param1);
        ov47_02259318(param3, v1);
        ov47_022591F8(param0);
        ov47_0225921C(param0);

        param0->unk_28 = 5;
        break;
    case 5:
        if (gSystem.newKeys & 0x0001) {
            PlaySE(1500);
            param0->unk_28++;
            break;
        }

        if (gSystem.newKeys & 0x0040) {
            if ((param0->unk_10 - 1) >= 0) {
                PlaySE(1504);
                param0->unk_10--;
            }
        } else if (gSystem.newKeys & 0x0080) {
            if ((param0->unk_10 + 1) < 3) {
                PlaySE(1504);
                param0->unk_10++;
            }
        }

        ov47_022591F8(param0);
        break;

    case 6:
        ov45_0222A7DC(param4->unk_04, param0->unk_2C.unk_04, param0->unk_10);
        ov45_0222F4D4(param0->unk_10);

        v1 = ov47_02258CEC(param1, 0, 96);

        ov47_02259318(param3, v1);
        ov47_02259384(param3);
        PlaySE(1405);
        param0->unk_28 = 7;
        break;
    case 7: {
        u32 v3;
        v3 = ov45_0222F524();

        if (v3 != UnkEnum_ov66_02233434_01) {
            PlaySE(1508);
            ov47_022593A0(param3);

            v1 = ov47_02258CEC(param1, 0, 97);
            ov47_022592B4(param3, v1);

            param0->unk_2A = 8;
            param0->unk_28 = 10;
        }
    } break;
    case 8:
        if (param0->unk_2C.unk_08 == 0) {
            v0 = 98;
        } else {
            v0 = 99;
        }

        v1 = ov47_02258CEC(param1, 0, v0);
        ov47_022592B4(param3, v1);
        param0->unk_2A = 9;
        param0->unk_28 = 10;
        break;
    case 9:
        ov47_022593CC(param3);
        return 1;
    case 10:
        if (ov47_022593B4(param3) == 1) {
            param0->unk_28 = param0->unk_2A;
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}
