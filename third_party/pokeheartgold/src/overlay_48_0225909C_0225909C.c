#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"

struct BgConfig;

struct G3DPipelineBuffers;

struct MessageLoader;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct SysTaskQueue;

struct WiFiHistory;

struct YesNoPrompt;

typedef struct WiFiHistory WiFiHistory;

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

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DModel {
    void *data;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Easy3DModel;

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct MessageLoader MessageLoader;

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

typedef struct StringTemplate MessageFormat;

typedef struct YesNoTouchMenu YesNoPrompt;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Easy3DObject unk_24;
    Easy3DModel unk_9C;
} UnkStruct_ov69_0225D71C;

typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Easy3DObject unk_B810[3];
    Easy3DModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;

typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov69_0225DA74 unk_04;
    WiFiHistory *wiFiHistory;
    Options *options;
    int unk_14;
    int unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    UnkStruct_ov69_0225D35C unk_20;
    UnkStruct_ov69_0225EF54 unk_1CC;
    UnkStruct_ov69_0225D71C unk_1DC;
    UnkStruct_ov69_0225CA7C unk_288;
    UnkStruct_ov69_0225DAEC unk_BC30;
    UnkStruct_ov69_0225DC48 unk_BC44;
    UnkStruct_ov69_0225E084 unk_BC70;
    UnkStruct_ov69_0225DDC8 unk_BF64;
} UnkStruct_ov69_0225CE64;

u32 ov48_02258D54(UnkStruct_ov69_0225CE64 *param0, int param1, int param2);

BOOL ov48_02259188(const UnkStruct_ov69_0225CE64 *param0, UnkStruct_ov69_0225E0C0 *param1);

BOOL ov48_022593B4(UnkStruct_ov69_0225CE64 *param0, u8 param1, u8 param2);

void ov48_02259DA0(UnkStruct_ov69_0225DC48 *param0);

BOOL ov48_0225A20C(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);

BOOL ov48_0225A244(const UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);

u32 ov48_0225909C(UnkStruct_ov69_0225CE64 *param0);

u32 ov48_0225909C(UnkStruct_ov69_0225CE64 *param0) {
    u32 v0;
    BOOL v1;

    ov48_02259DA0(&param0->unk_BC44);

    if (param0->unk_BC44.unk_00 & 0x0002) {
        PlaySE(1501);
        return 2;
    } else {
        if (gSystem.newKeys & 0x0400) {
            {
                UnkStruct_ov69_0225E0C0 v2;

                v1 = ov48_02259188(param0, &v2);

                if (v1 == 1) {
                    v1 = ov48_0225A244(&param0->unk_BC70, &v2);

                    if (v1 == 0) {
                        v1 = ov48_0225A20C(&param0->unk_BC70, &v2);

                        if (v1 == 0) {
                            (void)0;
                        } else {
                            ov48_022593B4(param0, v2.unk_04, v2.unk_08);
                            PlaySE(1494);
                        }
                    }
                }
            }
        } else {
            v0 = ov48_02258D54(param0, gSystem.newKeys, gSystem.heldKeys);
        }
    }

    return v0;
}
