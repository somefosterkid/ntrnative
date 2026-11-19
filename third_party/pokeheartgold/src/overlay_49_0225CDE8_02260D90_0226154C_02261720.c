#include "global.h"

#include "constants/heap.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "gf_3d_loader.h"

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct TextureResourceManager TextureResourceManager;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct PasswordWordBank PasswordWordBank;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef u16 charcode_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
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

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

struct UnkStruct_ov70_0225C894_t {
    UnkStruct_ov63_0222BCE8 *unk_00;
};

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
} UnkStruct_ov70_02265CCC;

struct PasswordWordBank {
    u32 length;
    u32 *words;
};



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
    TextureResourceManager *unk_08;
    BillboardList *unk_0C;
    UnkStruct_ov66_02234798 *unk_10;
    u32 unk_14;
    UnkStruct_ov66_02234958 unk_18;
    int unk_2C;
};

struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
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

BOOL ov49_02261DBC(UnkStruct_ov70_02265CCC *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, UnkStruct_ov70_0225CC54 *param3, u8 param4, int param5);

s32 ov45_0222A25C(WiFiPlazaSystem *, u32);

s32 ov45_0222A2A0(WiFiPlazaSystem *, u32);

s32 ov45_0222A374(WiFiPlazaSystem *);

const WiFiPlazaProfile *ov45_0222A578(WiFiPlazaSystem *, u32);

u32 ov45_0222A920(const WiFiPlazaProfile *profile);

s32 ov45_0222AD58(WiFiPlazaSystem *, u32);

s32 ov45_0222ADA8(WiFiPlazaSystem *, u32);

UnkStruct_ov70_0225CC54 *ov49_02258D70(s32, u32);

UnkStruct_ov70_0225CC54 *ov49_02258DAC(s32);

s32 ov49_02258EEC(s32, UnkStruct_ov70_0225CC54 *, s32);

s32 ov49_02259130(UnkStruct_ov70_0225CC54 *, s32);

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *);

s32 ov49_02259FF0(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A010(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A04C(UnkStruct_ov70_0225DEE8 *, u8, s32);

s32 ov49_0225A06C(UnkStruct_ov70_0225DEE8 *, u8, s32);

UnkStruct_ov70_02265CCC *ov49_0225EF40(UnkStruct_ov70_02263344 *, s32);

s32 ov49_0225EF68(UnkStruct_ov70_02263344 *);

UnkStruct_ov70_02265CCC *ov49_0225EF84(UnkStruct_ov70_02263344 *);

s32 ov49_0225EF88(UnkStruct_ov70_02263344 *);

s32 ov49_0225EF8C(UnkStruct_ov70_02263344 *, s32);

s32 ov49_0225EF98(s32, u32, s32 *, UnkStruct_ov70_02265CCC *);

s32 ov49_0225EFC4(s32, u32, s32 *, UnkStruct_ov70_02265CCC *);

extern s32 ov49_02269B48;

extern s32 ov49_02269B50;

extern s32 ov49_02269B58;

extern s32 ov49_02269B80;

s32 ov49_02261720(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

s32 ov49_02261720(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_02265CCC *sp14;
    s32 sp10;
    s32 temp_r0;
    UnkStruct_ov70_02263344 *sp8;
    UnkStruct_ov70_0225CC54 *temp_r7;
    WiFiPlazaSystem *temp_r6;
    s32 spC;
    s32 temp_r0_2;

    sp8 = param0;
    temp_r6 = ov49_02259FE8(param1);
    temp_r0 = ov49_02259FF0(param1);
    sp10 = temp_r0;
    spC = ov49_0225A010(param1);
    temp_r7 = ov49_02258D70(temp_r0, param2);
    ov49_02258DAC(temp_r0);
    sp14 = ov49_0225EF84(param0);
    temp_r0_2 = ov49_0225EF88(param0);
    switch (temp_r0_2) {
    case 0:
        sp14 = ov49_0225EF40(sp8, 8);
        ov49_0225EF8C(sp8, 1);
    case 1:
        if (ov45_0222A25C(temp_r6, param2) != 0) {
            ov49_0225EF68(sp8);
            ov49_02258EEC(sp10, temp_r7, 0);
            ov49_0225A06C(param1, (u8)param2, 1);
            ov49_0225A04C(param1, (u8)param2, 1);
            ov49_0225EF98(spC, param2, &ov49_02269B48, NULL);
        } else {
            ov45_0222A2A0(temp_r6, param2);
            if ((ov45_0222A374(temp_r6) == 1) && (ov45_0222ADA8(temp_r6, param2) != -1)) {
                ov49_02258EEC(sp10, temp_r7, 0);
                ov49_0225A06C(param1, (u8)param2, 1);
                ov49_0225A04C(param1, (u8)param2, 1);
                ov49_02259130(temp_r7, 0);
                ov49_0225EF68(sp8);
                ov49_0225EF98(spC, param2, &ov49_02269B50, sp14);
            } else {
                if (ov49_02261DBC(sp14, temp_r6, param1, temp_r7, (u8)(s32)ov45_0222A920(ov45_0222A578(temp_r6, param2)), 0) != 0) {
                    ov49_02258EEC(sp10, temp_r7, 0);
                    ov49_0225A06C(param1, (u8)param2, 1);
                    ov49_0225A04C(param1, (u8)param2, 1);
                    ov49_02259130(temp_r7, 0);
                    ov49_0225EFC4(spC, param2, &ov49_02269B58, sp14);
                } else if (ov45_0222AD58(temp_r6, param2) == 1) {
                    ov49_02258EEC(sp10, temp_r7, 0);
                    ov49_0225A04C(param1, (u8)param2, 1);
                    ov49_0225EFC4(spC, param2, &ov49_02269B80, NULL);
                }
            }
        }
        break;
    }
    return 0;
}
