#include "global.h"

#include "overlay_48.h"
#include "system.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_02026E30.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "msgdata.h"
#include "yes_no_prompt.h"


struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct WiFiHistory;
struct YesNoPrompt;
typedef struct WiFiHistory WiFiHistory;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
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
    Field3dObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;
typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Field3dObject unk_B810[3];
    Field3dModel unk_B978[3];
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
void ov48_022598CC(const UnkStruct_ov69_0225D71C *param0, VecFx32 *param1);
void ov48_022598DC(UnkStruct_ov69_0225D71C *param0, const VecFx32 *param1);
GXRgb ov48_02259CFC(const UnkStruct_ov69_0225DAEC *param0);
int ov48_02259E5C(const UnkStruct_ov69_0225DC48 *param0, u32 param1);
u32 ov48_0225909C(UnkStruct_ov69_0225CE64 *param0);
void ov48_02259C4C(UnkStruct_ov69_0225DAEC *param0);
BOOL ov48_02259C78(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225CA7C *param1);
void ov48_02259E78(UnkStruct_ov69_0225DC48 *param0);
void ov48_02259E90(UnkStruct_ov69_0225DC48 *param0);
void ov48_02259F48(UnkStruct_ov69_0225DDC8 *param0);
u32 ov48_02259F8C(UnkStruct_ov69_0225DDC8 *param0);
void ov48_02259FEC(UnkStruct_ov69_0225DDC8 *param0);
void ov48_0225A1D0(UnkStruct_ov69_0225E084 *param0);
void ov48_0225A1EC(UnkStruct_ov69_0225E084 *param0);
void ov48_0225A158(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2);
void ov48_022594D0(UnkStruct_ov69_0225D35C *param0);
void ov48_02259874(UnkStruct_ov69_0225D71C *param0);
void ov48_022599A0(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov69_0225D71C *param1);
void ov48_02259C44(const UnkStruct_ov69_0225DAEC *param0);
void ov48_022594DC(UnkStruct_ov69_0225D35C *param0);
u32 ov48_02258D54(UnkStruct_ov69_0225CE64 *param0, int param1, int param2);
BOOL ov48_02258F64(UnkStruct_ov69_0225CE64 *param0);
void ov48_02259030(UnkStruct_ov69_0225CE64 *param0);
void ov48_02259050(UnkStruct_ov69_0225CE64 *param0);

void ov48_02258CE4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v0 = 0;
    v1 = 0;
    v2 = 0;
    v3 = 0;
    if (gSystem.touchX != 0xFFFF) {
        v2 = gSystem.touchX - arg0;
        if (v2 < 0) {
            v2 ^= -1;
            v0 = 0x10;
        } else if (v2 > 0) {
            v0 = 0x20;
        }
    }
    *arg2 = v0;
    *arg3 = v2 & 0x3F;
    if (gSystem.touchY != 0xFFFF) {
        v3 = gSystem.touchY - arg1;
        if (v3 < 0) {
            v3 ^= -1;
            v1 = 0x80;
        } else if (v3 > 0) {
            v1 = 0x40;
        }
    }
    *arg4 = v1;
    *arg5 = v3 & 0x3F;
}

u32 ov48_02258D54(UnkStruct_ov69_0225CE64 *param0, int param1, int param2) {
    u16 v0;
    u16 v1;
    s16 v2;
    s16 v3;
    u32 v4 = 0;
    VecFx32 v5;
    int v6;
    int v7, v8;
    u32 v9;

    ov48_022598CC(&param0->unk_1DC, &v5);

    v6 = ov48_02259E5C(&param0->unk_BC44, 0);
    v7 = ov48_02259E5C(&param0->unk_BC44, 1);
    v8 = ov48_02259E5C(&param0->unk_BC44, 2);
    v9 = ov48_02259CFC(&param0->unk_BC30);

    v2 = v5.x;
    v3 = v5.y;

    if ((param1 & 0x0001) || (v6 & 0x0001)) {
        if (param0->unk_04.unk_00_0 == 1) {
            v4 = 1;
        }

        return v4;
    }

    if (v9 == 0) {
        if ((v7) || (v8)) {
            v0 = (0x200 / 6) * v7;
            v1 = (0x200 / 6) * v8;
        } else {
            v0 = 0x200;
            v1 = 0x200;
        }
    } else {
        if ((v7) || (v8)) {
            v0 = (0x20 / 3) * v7;
            v1 = (0x20 / 3) * v8;
        } else {
            v0 = 0x20;
            v1 = 0x20;
        }
    }

    if ((param2 & 0x0020) || (v6 & 0x0020)) {
        if (param0->unk_04.unk_00_0 == 1) {
            v5.y += v0;
        } else {
            if (v3 < (s16)0xd820) {
                v5.y += v0;
            }
        }
    }

    if ((param2 & 0x0010) || (v6 & 0x0010)) {
        if (param0->unk_04.unk_00_0 == 1) {
            v5.y -= v0;
        } else {
            if (v3 > (s16)0xcc80) {
                v5.y -= v0;
            }
        }
    }

    if ((param2 & 0x0040) || (v6 & 0x0040)) {
        if (param0->unk_04.unk_00_0 == 1) {
            if ((v2 + v1) < (0x4000 - 0x200)) {
                v5.x += v1;
            } else {
                v5.x = (0x4000 - 0x200);
            }
        } else {
            if (v2 < (s16)0x2020) {
                v5.x += v1;
            }
        }
    }

    if ((param2 & 0x0080) || (v6 & 0x0080)) {
        if (param0->unk_04.unk_00_0 == 1) {
            if ((v2 - v1) > (-0x4000 + 0x200)) {
                v5.x -= v1;
            } else {
                v5.x = (-0x4000 + 0x200);
            }
        } else {
            if (v2 > (s16)0x1300) {
                v5.x -= v1;
            }
        }
    }

    ov48_022598DC(&param0->unk_1DC, &v5);
    return v4;
}

void ov48_02258F0C(VecFx32 *param0) {
    if (param0->x >= 0) {
        param0->x = param0->x % 0xffff;
    } else {
        param0->x = param0->x + (0xffff * (((((param0->x) < 0) ? (-(param0->x)) : (param0->x)) / 0xffff) + 1));
    }

    if (param0->y >= 0) {
        param0->y = param0->y % 0xffff;
    } else {
        param0->y = param0->y + (0xffff * (((((param0->y) < 0) ? (-(param0->y)) : (param0->y)) / 0xffff) + 1));
    }
}

BOOL ov48_02258F64(UnkStruct_ov69_0225CE64 *param0) {
    u32 v0;

    switch (param0->unk_00) {
    case 0:
        v0 = ov48_0225909C(param0);

        switch (v0) {
        case 0:
            break;
        case 1:
            ov48_02259C4C(&param0->unk_BC30);
            param0->unk_00 = 1;
            break;
        case 2:
            param0->unk_00 = 2;
            break;
        }
        break;
    case 1:
        v0 = ov48_02259C78(&param0->unk_BC30, &param0->unk_288);

        if (v0 == 1) {
            param0->unk_00 = 0;
        }
        break;
    case 2:
        ov48_02259F48(&param0->unk_BF64);
        ov48_0225A1D0(&param0->unk_BC70);
        ov48_02259E78(&param0->unk_BC44);
        param0->unk_00++;
        break;
    case 3: {
        u32 v1;

        v1 = ov48_02259F8C(&param0->unk_BF64);

        switch (v1) {
        case 0:
            break;
        case 2:
            ov48_0225A1EC(&param0->unk_BC70);
            ov48_02259FEC(&param0->unk_BF64);
            ov48_02259E90(&param0->unk_BC44);
            param0->unk_00 = 0;
            break;
        default:
        case 1:
            return 1;
        }
    } break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

void ov48_02259030(UnkStruct_ov69_0225CE64 *param0) {
    switch (param0->unk_02) {
    case 0:
        ov48_0225A158(&param0->unk_BC70, &param0->unk_1CC, &param0->unk_20);
        break;
    }
}

void ov48_02259050(UnkStruct_ov69_0225CE64 *param0) {
    Thunk_G3X_Reset();

    ov48_02259C44(&param0->unk_BC30);
    ov48_02259874(&param0->unk_1DC);
    ov48_022599A0(&param0->unk_288, &param0->unk_1DC);
    ov48_022594D0(&param0->unk_20);

    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}

void ov48_02259090(void *param0) {
    UnkStruct_ov69_0225CE64 *v0 = param0;
    ov48_022594DC(&v0->unk_20);
}
