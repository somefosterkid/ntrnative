#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "msgdata.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

s32 ov48_0225A95C(void *, void *);

void ov48_0225A338(u8 *arg0, u8 *arg1);

struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
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
BOOL ov48_0225A30C(const UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225E0C0 *param1, u32 param2);
void ov48_0225A790(UnkStruct_ov69_0225E084 *param0, u32 param1, const UnkStruct_ov69_0225E0C0 *param2, UnkStruct_ov69_0225EF54 *param3);
void ov48_0225AA38(UnkStruct_ov69_0225E7DC *param0, u32 param1, s8 param2);
void ov48_0225AA50(UnkStruct_ov69_0225E7DC *param0);
void ov48_0225ACAC(UnkStruct_ov69_0225EB60 *param0);
void ov48_0225ACD8(UnkStruct_ov69_0225EB60 *param0, u8 param1, u8 param2, u8 param3);
u16 ov48_0225B038(u32 param0);
void ov48_0225A354(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2);

struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
void ov48_0225A43C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, int param2);
void ov48_0225A41C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

void ov48_0225A428(UnkStruct_ov69_0225E084 *param0);

void ov48_0225A430(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

void ov48_0225A338(u8 *arg0, u8 *arg1) {
    s32 v0;
    u8 *v1;

    v0 = 0;
    v1 = arg0 + 0xA0;
    do {
        ov48_0225A95C(v1, arg1);
        v0 += 1;
        v1 += 0x28;
    } while (v0 < 5);
}

void ov48_0225A354(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2) {
    memset(param0->unk_40, 0, sizeof(UnkStruct_ov69_0225E084_sub1) * (3 * 6));

    param0->unk_3C = 0;
    param0->unk_3E = 0;

    {
        int v0;
        int v1;
        int v2;
        BOOL v3;
        int v4;
        UnkStruct_ov69_0225E0C0 v5;

        for (v0 = 0; v0 < 5; v0++) {
            ov48_0225AA50(&param0->unk_A0[v0]);
        }

        ov48_0225ACAC(&param0->unk_1B8);

        v4 = -1;

        for (v0 = 0; v0 < 3; v0++) {
            v1 = 2 - v0;
            v3 = ov48_0225A30C(param0, &v5, v0);

            if (v3 == 1) {
                v2 = ov48_0225B038(v5.unk_00);

                ov48_0225AA38(&param0->unk_A0[v2], v1, 1);
                ov48_0225A790(param0, v1, &v5, param1);

                param0->unk_88[v1] = v2;
                ov48_0225ACD8(&param0->unk_1B8, v1, v5.unk_00, v4);
                param0->unk_3E += 6;

                v4 = v5.unk_00;
            } else {
                param0->unk_88[v1] = 5;
                v4 = -1;
            }
        }
    }
}

void ov48_0225A41C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov48_0225A43C(param0, param1, 2);
}

void ov48_0225A428(UnkStruct_ov69_0225E084 *param0) {
    int v0 = param0->unk_3C / 2;
    param0->unk_3C = v0 * 1;
}

void ov48_0225A430(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov48_0225A43C(param0, param1, 1);
}
