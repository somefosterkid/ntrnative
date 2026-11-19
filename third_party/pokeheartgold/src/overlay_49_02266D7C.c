#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "screen_fade_internal.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct SysTaskQueue;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02269204_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

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

typedef struct Easy3DAnim {
    void *data;
    void *anim;
    NNSG3dAnmObj *animObj;
    fx32 frame;
    BOOL dataBorrowed;
} Easy3DAnim;

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

void ov49_02258E04(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1, int param2);

UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

void ov49_02259148(UnkStruct_ov70_0225CC54 *param0, const VecFx32 *param1);

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

void ov49_02259160(UnkStruct_ov70_0225CC54 *param0, int param1);

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

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov70_0226E2EC;

typedef struct {
    s32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    fx32 unk_24;
} UnkStruct_ov70_022692B4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
    fx32 unk_08;
} UnkStruct_ov70_0226949C;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov70_0226E5A4;

typedef struct {
    s16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov70_022692B4 unk_08[4];
    UnkStruct_ov70_0226949C unk_A8[4];
    Easy3DObject *unk_D8[4];
    const UnkStruct_ov70_0225CC54 *unk_E8;
} UnkStruct_ov70_0226BD64;

typedef union {
    struct {
        s8 unk_00;
        s8 unk_01;
        s16 unk_02;
        fx32 unk_04;
        u8 unk_08[3];
        u8 unk_0B;
        u8 unk_0C[3];
        u8 unk_0F;
        u8 unk_10;
        u8 unk_11;
        u8 unk_12[2];
        UnkStruct_ov70_0226949C unk_14[3];
        UnkStruct_ov70_022692B4 unk_38[3];
        UnkStruct_ov70_022692B4 unk_B0;
        UnkStruct_ov70_0226949C unk_D8;
    } val1;
    struct {
        s32 unk_00;
        s32 unk_04[3];
    } val2;
    struct {
        s8 unk_00;
        s8 unk_01;
        u8 unk_02;
        u8 unk_03;
        s8 unk_04[16];
        UnkStruct_ov70_022692B4 unk_14[16];
    } val3;
    struct {
        s8 unk_00;
        s8 unk_01;
        s8 unk_02[2];
    } val4;
    struct {
        s32 unk_00;
    } val5;
    struct {
        s16 unk_00;
        s16 unk_02;
    } val6;
    struct {
        UnkStruct_ov70_0226BD64 unk_00[4];
        const UnkStruct_ov70_0226E2EC *unk_3B0;
        u16 unk_3B4;
        u16 unk_3B6;
        u16 unk_3B8;
        u16 unk_3BA;
    } val7;
} UnkStruct_ov70_0226BC40;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    u32 unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    Easy3DObject unk_0C[18];
    const UnkStruct_ov70_0226E5A4 *unk_87C[18];
    fx32 unk_8C4[18][2];
    UnkStruct_ov70_0226BC40 unk_954;
} UnkStruct_ov70_02269800;

struct UnkStruct_ov70_02269204_t {
    WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_0225CA20 *unk_04;
    UnkStruct_ov70_02260AD4 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    UnkStruct_ov70_02269800 unk_10[20];
    Easy3DModel unk_10550[15];
    void *unk_10640[39];
    Easy3DAnim unk_106DC[16];
    NNSFndAllocator unk_1081C;
};

BOOL ov49_02265434(UnkStruct_ov70_022692B4 *param0, s32 param1);

void ov49_022655E0(const UnkStruct_ov70_022692B4 *param0, fx32 *param1, fx32 *param2, fx32 *param3);

void ov49_02265628(UnkStruct_ov70_0226949C *param0);

void ov49_02265660(const UnkStruct_ov70_0226949C *param0, fx32 *param1);

void ov49_0226747C(UnkStruct_ov70_02269800 *param0);

void ov49_0226789C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2);

BOOL ov49_02266D7C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266F14(void *param_1, void *param_2);
void ov49_02267074(u8 *param_1, u8 *param_2);
void ov49_02266EF8(u8 *param_1, u8 *param_2);

BOOL ov49_02266D7C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    BOOL v0;
    VecFx32 v1;
    fx32 v2;
    int v3;

    v0 = ov49_02265434(&param1->unk_954.val1.unk_B0, param1->unk_954.val1.unk_02);
    ov49_02265628(&param1->unk_954.val1.unk_D8);
    param1->unk_954.val1.unk_02++;

    ov49_02259154(param1->unk_08, &v1);
    ov49_022655E0(&param1->unk_954.val1.unk_B0, &v1.x, &v1.y, &v1.z);
    ov49_02265660(&param1->unk_954.val1.unk_D8, &v2);

    if (v0 == 0) {
        v1.x += v2;
    }

    ov49_02259148(param1->unk_08, &v1);
    ov49_0226747C(param1);

    for (v3 = 0; v3 < param1->unk_954.val1.unk_01; v3++) {
        ov49_0226789C(param0, param1, v3);
    }

    if (param1->unk_08 == ov49_02258DAC(param0->unk_04)) {
        u32 v4 = 4;

        if (gSystem.newKeys & 0x0040) {
            v4 = 0;
        }

        if (gSystem.newKeys & 0x0080) {
            v4 = 1;
        }

        if (gSystem.newKeys & 0x0010) {
            v4 = 3;
        }

        if (gSystem.newKeys & 0x0020) {
            v4 = 2;
        }

        if (v4 != 4) {
            ov49_02259160(param1->unk_08, v4);
            ov49_02258E04(param1->unk_08, ov49_02258E34(param1->unk_08), v4);
        }
    }

    return v0;
}

void ov49_02267674(UnkStruct_ov70_02269800 *param0);

BOOL ov49_02266E78(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266E78(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    BOOL v0;
    VecFx32 v1;
    int v2;

    v0 = ov49_02265434(&param1->unk_954.val1.unk_B0, param1->unk_954.val1.unk_02);
    ov49_02265628(&param1->unk_954.val1.unk_D8);
    param1->unk_954.val1.unk_02++;

    ov49_02259154(param1->unk_08, &v1);
    ov49_022655E0(&param1->unk_954.val1.unk_B0, &v1.x, &v1.y, &v1.z);
    ov49_02259148(param1->unk_08, &v1);
    ov49_02267674(param1);

    for (v2 = 0; v2 < param1->unk_954.val1.unk_01; v2++) {
        ov49_0226789C(param0, param1, v2);
    }

    return v0;
}

void ov49_02266EF8(u8 *param_1, u8 *param_2) {
    unsigned char new_var2;
    u8 *new_var;
    param_2++;
    param_2--;
    new_var = param_2;
    if ((((((((((((((*((s8 *)(new_var + 0x965))) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & (new_var2 = 0xFFFF)) == 0) {
        ov49_02266F14(param_1, new_var);
        return;
    }
    ov49_02267074(param_1, new_var);
    return;
}
