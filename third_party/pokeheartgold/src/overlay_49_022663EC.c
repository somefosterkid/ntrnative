#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "easy_chat.h"
#include "gf_3d_loader.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;


typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef u16 charcode_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
};

typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

struct UnkStruct_ov66_02234548_t {
    GF_2DGfxRawResMan *unk_00;
    GF_2DGfxRawResMan *unk_04;
    GF_3DGfxRawResMan *unk_08;
    BillboardList *unk_0C;
    UnkStruct_ov66_02234798 *unk_10;
    u32 unk_14;
    UnkStruct_ov66_02234958 unk_18;
    int unk_2C;
};

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
    Field3dObject *unk_D8[4];
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
    Field3dObject unk_0C[18];
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
    Field3dModel unk_10550[15];
    void *unk_10640[39];
    Field3DModelAnimation unk_106DC[16];
    NNSFndAllocator unk_1081C;
};

struct UnkStruct_0207E060_t {
    u16 unk_00[6];
    u16 unk_0C[6];
    u8 unk_18;
    u8 language;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 unk_1F;
    u8 unk_20;
    u8 unk_21;
    u8 unk_22;
};

s32 ov49_02265980(UnkStruct_ov70_02269204 *, UnkStruct_ov70_02269800 *, s32, void *);

s32 ov49_02267D98(UnkStruct_ov70_0226BC40 *, s32 *, s32, s32, s32, UnkStruct_ov70_02269800 *);

extern s32 ov49_0226A46C;

extern void *ov49_0226A70C;

extern u8 ov49_0226A7B8;

void ov49_022663EC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

extern s32 ov49_0226A4B4;
extern u8 ov49_0226A7C8;
void ov49_0226643C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);
extern s32 ov49_0226A464;
extern u8 ov49_0226A7A8;
void ov49_0226648C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

extern s32 ov49_0226A49C;
void ov49_022664DC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);
extern s32 ov49_0226A4C0;
void ov49_0226652C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);
void ov49_02267C8C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);
extern s32 ov49_0226A47C;
void ov49_0226657C(UnkStruct_ov70_02269204 *arg0, UnkStruct_ov70_02269800 *arg1);
extern s32 ov49_0226A45C;
void ov49_022665D0(UnkStruct_ov70_02269204 *arg0, UnkStruct_ov70_02269800 *arg1);
extern s32 ov49_0226A474;
void ov49_02266624(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_022663EC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7B8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A46C, 2, 0x21, 0, param1);
}

void ov49_0226643C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7C8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A4B4, 3, 0x21, 0, param1);
}

void ov49_0226648C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7A8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A464, 2, 0x21, 1, param1);
}

void ov49_022664DC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7B8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A49C, 3, 0x29, 1, param1);
}

void ov49_0226652C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7C8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A4C0, 3, 0x21, 1, param1);
}

void ov49_0226657C(UnkStruct_ov70_02269204 *arg0, UnkStruct_ov70_02269800 *arg1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7A8;
    v0 = arg0;
    v1 = 0;
    do {
        ov49_02265980(v0, arg1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&arg1->unk_954, &ov49_0226A47C, 2, 0x21, 2, arg1);
    ov49_02267C8C(v0, arg1);
}

void ov49_022665D0(UnkStruct_ov70_02269204 *arg0, UnkStruct_ov70_02269800 *arg1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7B8;
    v0 = arg0;
    v1 = 0;
    do {
        ov49_02265980(v0, arg1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&arg1->unk_954, &ov49_0226A45C, 2, 0x21, 2, arg1);
    ov49_02267C8C(v0, arg1);
}

void ov49_02266624(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    UnkStruct_ov70_02269204 *v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7C8;
    v0 = param0;
    v1 = 0;
    do {
        ov49_02265980(v0, param1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(&param1->unk_954, &ov49_0226A474, 2, 0x21, 2, param1);
    ov49_02267C8C(v0, param1);
}
