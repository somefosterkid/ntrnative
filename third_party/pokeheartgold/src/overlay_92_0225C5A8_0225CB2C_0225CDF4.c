#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "save.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_020379A0.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c
typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    struct PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;

struct ApplicationManager;

struct G3DPipelineBuffers;

struct PlayerProfile;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov66_0222DFF8_t;






typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    BOOL unk_38;
    u32 unk_3C;
} UnkStruct_ov66_02231134;



typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;

typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;

typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    G3DPipelineBuffers *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;

typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;

typedef struct {
    u32 unk_00;
} UnkStruct_ov116_02260490;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u8 padding_08[4];
} UnkStruct_ov116_02262A8C_sub1;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20[4];
    int unk_28[4];
    int unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    UnkStruct_ov116_0226469C unk_74;
    UnkStruct_ov116_0226469C unk_8C;
    BOOL unk_A4;
    BOOL unk_A8;
    BOOL unk_AC;
    BOOL unk_B0;
    BOOL unk_B4;
    s16 unk_B8;
    s16 unk_BA;
} UnkStruct_ov116_02262A8C_sub2;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

typedef struct {
    Easy3DObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;

typedef struct {
    int unk_00;
    int unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C;
    UnkStruct_ov116_0226469C unk_10;
    int unk_28;
    int *unk_2C;
    BOOL unk_30;
    UnkStruct_ov116_0226501C *unk_34;
    UnkStruct_ov116_0226501C *unk_38;
    UnkStruct_ov116_022649E4 *unk_3C;
    int unk_40;
} UnkStruct_ov116_02262D08;

typedef struct {
    BOOL unk_00;
    int unk_04;
    ManagedSprite *unk_08[3];
    UnkStruct_ov116_0226469C unk_14[3][2];
    UnkStruct_ov116_022649E4 *unk_A4;
} UnkStruct_ov116_022647BC;

typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;

typedef struct {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;

typedef struct {
    ManagedSprite *unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[6];
    int unk_10;
    s16 unk_14;
    s16 unk_16;
    int unk_18;
    int unk_1C;
    UnkStruct_ov116_0226469C unk_20;
    UnkStruct_ov116_0226469C unk_38;
} UnkStruct_ov116_02265D84;

typedef struct {
    UnkStruct_ov116_02265D84 unk_00[2];
    UnkStruct_ov116_02265D84 unk_A0[2];
    int unk_140;
    int unk_144;
    int unk_148;
    int unk_14C;
} UnkStruct_ov116_02265AA4;

typedef struct {
    UnkStruct_ov116_02265AA4 unk_00;
    u8 padding_04[4];
    SysTask *unk_08;
    int *unk_0C;
    int *unk_10;
    int *unk_14;
    int unk_18;
    int unk_1C;
    u8 padding_20[4];
    int unk_24;
    BOOL unk_28;
    BOOL unk_2C;
    UnkStruct_ov116_022649E4 *unk_30;
    UnkStruct_ov116_0226469C unk_34[5];
} UnkStruct_ov116_022660A8;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    u8 padding_10[4];
    UnkStruct_ov116_022649E4 *unk_14;
    u8 padding_18[4];
    ManagedSprite *unk_1C[3];
    ManagedSprite *unk_20[3];
    ManagedSprite *unk_24[3];
    ManagedSprite *unk_28[36];
    ManagedSprite *unk_B8[4];
    ManagedSprite *unk_C8[2];
    ManagedSprite *unk_D0[2];
    ManagedSprite *unk_D8;
    u16 unk_DC[4];
    u8 padding_E4[12];
    int unk_F0;
    int unk_F4;
    int unk_F8;
    UnkStruct_ov116_0226501C unk_FC;
    UnkStruct_ov116_0226501C unk_308[3];
    UnkStruct_ov116_0226501C unk_92C;
    UnkStruct_ov116_0226501C unk_B38[8];
    UnkStruct_ov116_0226501C unk_1B98;
    UnkStruct_ov116_0226501C unk_1DA4;
    VecFx32 unk_1FB0;
    UnkStruct_ov116_02262A8C_sub1 unk_1FBC;
    Window unk_1FC8;
    Window unk_1FD8[4];
    UnkStruct_ov116_022660A8 unk_2018;
    UnkStruct_ov116_0226534C unk_20C4;
    UnkStruct_ov116_022647BC unk_214C[8];
    UnkStruct_ov116_02262D08 unk_268C[4];
    UnkStruct_ov116_02262A8C_sub2 unk_279C;
    UnkStruct_ov116_02260490 unk_2858[4];
    u8 padding_2868[4];
    u32 unk_286C;
    int unk_2870;
    BOOL unk_2874;
} UnkStruct_ov116_02262A8C;

typedef struct {
    UnkStruct_ov116_02262A44 *unk_00;
    UnkStruct_ov116_02262A8C *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    UnkStruct_ov116_0226048C unk_14;
    UnkStruct_ov116_02260494 unk_1C;
    UnkStruct_ov116_02260498 unk_24;
    u32 unk_44;
    UnkStruct_ov116_022649E4 unk_48;
    u32 unk_78;
    UnkStruct_ov114_0225CAD4 *unk_7C;
    UnkStruct_ov66_02231134 *unk_80;
    UnkStruct_ov114_0225C76C unk_84;
    UnkStruct_ov114_0225D678 *unk_94;
    UnkStruct_ov114_0225C9A8 unk_98;
    UnkStruct_ov114_0225E854 *unk_B0;
    int unk_B4[4];
    BOOL unk_C4;
} UnkStruct_ov116_0226139C;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

void *OverlayManager_GetData(ApplicationManager *appMan, int *);

BOOL ov92_0225D8C4(UnkStruct_ov116_0226139C *param0);

void ov00_021E6A4C(void);

UnkStruct_ov114_0225D678 *ov90_02258920(const UnkStruct_ov114_0225C76C *param0, u32 heapID);

void ov90_02258938(UnkStruct_ov114_0225D678 *param0);

BOOL ov90_022589BC(const UnkStruct_ov114_0225D678 *param0);

int ov90_022589CC(const UnkStruct_ov114_0225D678 *param0);

UnkStruct_ov114_0225E854 *ov90_022589EC(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225C9A8 *param1, u32 heapID);

void ov90_02258A04(UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258AA0(const UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258AA4(const UnkStruct_ov114_0225E854 *param0);

void ov90_02258AA8(UnkStruct_ov114_0225C9A8 *param0, u32 param1);

void ov90_02258C8C(UnkStruct_ov114_0225CAD4 *param0);

void ov90_02258CB0(UnkStruct_ov114_0225CAD4 *param0);

BOOL ov90_02258CE0(const UnkStruct_ov114_0225CAD4 *param0);

void ov92_0225DA40(UnkStruct_ov116_0226139C *param0);

void ov92_0225E008(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E070(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E100(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E1A8(UnkStruct_ov116_0226139C *param0, UnkStruct_ov116_02262A8C *param1);

void ov92_0225E360(UnkStruct_ov116_02262A8C *param0);

BOOL ov92_0225EB70(UnkStruct_ov116_02262A44 *param0);

void ov92_0225FC2C(UnkStruct_ov116_02262A8C *param0);

int ov92_0226077C(UnkStruct_ov116_0226139C *param0, int param1);

void ov92_0226156C(UnkStruct_ov116_02262A8C *param0);

void ov92_02261B18(UnkStruct_ov116_02262A8C *param0);

BOOL sub_02037030(int cmd, const void *data, int size);

BOOL ov92_0225D868(int param0);

void ov92_0225D88C(UnkStruct_ov116_0226139C *param0, BOOL param1, int param2, int *param3);

void ov92_0225D1FC(UnkStruct_ov116_0226139C *param0);

void ov92_0225D344(UnkStruct_ov116_0226139C *param0);

u32 ov92_0225D8E4(UnkStruct_ov116_0226139C *param0);

void ov92_0225C5C4(UnkStruct_ov116_0226139C *param0);

BOOL ov92_0225CC6C(UnkStruct_ov116_02262A8C *param0);

int ov92_0225CDF4(ApplicationManager *appMan, int *param1);

int ov92_0225CDF4(ApplicationManager *appMan, int *param1) {
    UnkStruct_ov116_0226139C *v0 = OverlayManager_GetData(appMan, param1);
    BOOL v1 = 0;
    u32 v2 = ov92_0225D8E4(v0);

    switch (v2) {
    case 1:
        return 0;
    case 2:
        return 1;
    case 0:
    default:
        break;
    }

    switch (*param1) {
    case 0:
        v1 = 1;
        v0->unk_94 = ov90_02258920(&v0->unk_84, HEAP_ID_113);
        ov92_0225D88C(v0, v1, 1, param1);
        break;
    case 1:
        v1 = ov90_022589BC(v0->unk_94);
        ov92_0225D88C(v0, v1, 2, param1);
        break;
    case 2:
        v1 = 1;
        v0->unk_14.unk_00 = ov90_022589CC(v0->unk_94);
        ov90_02258938(v0->unk_94);
        v0->unk_94 = 0L;
        ov92_0225C5C4(v0);
        ov92_0225D88C(v0, v1, 3, param1);
        break;
    case 3:
        v1 = ov92_0225D868(27);
        ov92_0225E1A8(v0, v0->unk_04);
        ov92_0225D88C(v0, v1, 4, param1);
        break;
    case 4:
        v1 = IsPaletteFadeFinished();
        ov92_0225D88C(v0, v1, 5, param1);
        break;
    case 5:
        v1 = 1;

        if (v1 == 0) {
            break;
        }

        ov92_0225E070(v0->unk_04);
        ov92_0226156C(v0->unk_04);
        ov92_02261B18(v0->unk_04);
        ov92_0225D88C(v0, v1, 6, param1);
        break;
    case 6:
        v1 = ov92_0225CC6C(v0->unk_04);

        if (v1) {
            if (ov92_0225D8C4(v0) == 1) {
                v0->unk_14.unk_04 = v0->unk_78;
                sub_02037030(22, &v0->unk_14, sizeof(UnkStruct_ov116_0226048C));
            }
        }

        ov92_0225D88C(v0, v1, 7, param1);
        break;
    case 7:
        v1 = v0->unk_08;
        ov92_0225D88C(v0, v1, 8, param1);
        break;
    case 8:
        v1 = 1;
        ov90_02258C8C(v0->unk_7C);
        ov92_0225D88C(v0, v1, 9, param1);
        break;
    case 9:
        v1 = 1;
        v1 = ov90_02258CE0(v0->unk_7C);

        if (v1) {
            ov92_0225E360(v0->unk_04);
            ov92_0225E100(v0->unk_04);
        }

        ov92_0225D88C(v0, v1, 10, param1);
        break;
    case 10:
        v1 = v0->unk_0C;

        if (v0->unk_0C) {
            if (v0->unk_04->unk_279C.unk_04 < ((30 * 40) - 0)) {
                v1 = 0;
            }
        }

        if (v1 == 0) {
            if (ov92_0225D8C4(v0) == 1) {
                if (ov92_0225EB70(v0->unk_00) == 0) {
                    StopSE(1417, 0);
                    sub_02037030(23, 0L, 0);
                }
            }
            ov92_0225FC2C(v0->unk_04);
        }
        ov92_0225D88C(v0, v1, 11, param1);
        break;
    case 11:
        v1 = 1;
        StopSE(1419, 0);
        StopSE(1417, 0);
        ov90_02258CB0(v0->unk_7C);
        ov92_0225D88C(v0, v1, 12, param1);
        break;
    case 12:
        v1 = ov90_02258CE0(v0->unk_7C);
        ov92_0225D88C(v0, v1, 13, param1);
        break;
    case 13:
        v1 = 1;
        v0->unk_1C.unk_00 = (v0->unk_04->unk_279C.unk_10 / 10) * 100;

        if (v0->unk_1C.unk_00 >= 100000) {
            v0->unk_1C.unk_00 = 100000;
        }

        if (v0->unk_04->unk_279C.unk_B4 == 0) {
            v0->unk_1C.unk_00 = 1;
        }

        sub_02037030(25, &v0->unk_1C, sizeof(UnkStruct_ov116_02260494));
        ov92_0225D88C(v0, v1, 14, param1);
        break;
    case 14:
        v1 = 0;
        {
            int v3;
            int v4 = 0;

            for (v3 = 0; v3 < 4; v3++) {
                if (v0->unk_24.unk_00[v3].unk_00 != 0) {
                    v4++;
                }
            }

            if (v4 == v0->unk_04->unk_04) {
                v1 = 1;
            }
        }
        ov92_0225D88C(v0, v1, 15, param1);
        break;
    case 15:
        v1 = ov92_0225D868(26);
        ov92_0225D88C(v0, v1, 16, param1);
        break;
    case 16:
        v1 = IsPaletteFadeFinished();

        if (v1) {
            ov92_0225D1FC(v0);
        }

        ov92_0225D88C(v0, v1, 17, param1);
        break;
    case 17:
        v1 = 1;
        {
            int v5;
            int v6;

            for (v5 = 0; v5 < 4; v5++) {
                v6 = ov92_0226077C(v0, v5);

                if (v6 == 0xFF) {
                    continue;
                }

                v0->unk_98.unk_00[v6] = v0->unk_24.unk_00[v5].unk_00;

                if (v0->unk_98.unk_00[v6] <= 1) {
                    v0->unk_98.unk_00[v6] = 0;
                }
            }

            ov90_02258AA8(&v0->unk_98, v0->unk_84.unk_08);
        }
        v0->unk_B0 = ov90_022589EC(&v0->unk_84, &v0->unk_98, HEAP_ID_113);
        ov92_0225D88C(v0, v1, 18, param1);
        break;
    case 18:
        v1 = ov90_02258AA0(v0->unk_B0);

        if (v1) {
            BOOL v7 = ov90_02258AA4(v0->unk_B0);

            ov90_02258A04(v0->unk_B0);
            v0->unk_B0 = 0L;

            if (v7) {
                v0->unk_C4 = 1;
                ov92_0225D344(v0);
                ov92_0225D88C(v0, v1, 0, param1);

                if (v0->unk_80->unk_3C) {
                    ov00_021E6A4C();
                }
                break;
            }
        }
        ov92_0225D88C(v0, v1, 19, param1);
        break;
    case 19:
        v1 = 1;
        sub_02037AC0(27);

        if (v0->unk_80->unk_3C) {
            ov00_021E6A4C();
        }

        ov92_0225D88C(v0, v1, 20, param1);
        break;
    case 20:
    default:
        v1 = sub_02037B38(27);
        return (v1 == 1) ? 1 : 0;
        break;
    }

    if ((*param1 >= 3) && (*param1 < 16)) {
        ov92_0225E008(v0->unk_04);
        ov92_0225DA40(v0);
    }

    return 0;
}
