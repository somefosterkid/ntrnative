#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"
#include "unk_020379A0.h"
#include "g2d_renderer.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "touch_pad_data_buffer.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"

typedef struct TrainerInfo TrainerInfo;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

typedef u16 charcode_t;

struct TrainerInfo {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
};

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov115_022612BC;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const TrainerInfo *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov115_02261304;

typedef struct {
    u32 unk_00;
    s32 unk_04;
    s32 unk_08;
    Window unk_0C;
} UnkStruct_ov115_022617E8;

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[6];
    u16 unk_210;
    u16 unk_212;
    Field3DModelAnimation unk_214[1];
    fx32 unk_228;
    u16 unk_22C;
    u16 unk_22E;
} UnkStruct_ov115_02263DF8;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    VecFx32 unk_04;
    fx32 unk_10;
    VecFx32 unk_14;
    s32 unk_20;
    BOOL unk_24;
} UnkStruct_ov115_02262134;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    UnkStruct_ov115_02262134 unk_04;
    VecFx32 unk_2C;
    s16 unk_38;
    u16 unk_3A;
    BOOL unk_3C;
    fx32 unk_40;
} UnkStruct_ov115_022622F8;

typedef struct {
    u32 unk_00;
    NNSG2dSVec2 unk_04[8];
    u16 unk_24;
    u16 unk_26;
    u32 unk_28;
} UnkStruct_ov115_02262CA4;

typedef struct {
    Field3dModel unk_00[5];
    Field3dModel unk_50;
    SpriteResource *unk_60[4];
    SpriteResourcesHeader unk_70;
} UnkStruct_ov115_02264A40;

typedef struct {
    Sprite *unk_00;
    Field3dObject unk_04;
    Field3dObject unk_7C;
    BOOL unk_F4;
    const UnkStruct_ov115_022622F8 *unk_F8;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    s16 unk_104;
} UnkStruct_ov115_022647A0;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04;
} UnkStruct_ov115_02264BA0;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04;
    VecFx32 unk_08;
    s32 unk_14;
    BOOL unk_18;
} UnkStruct_ov115_02264D80;

typedef struct {
    UnkStruct_ov115_02264D80 unk_00[8];
} UnkStruct_ov115_02264C90;

typedef struct {
    Sprite *unk_00[3];
    u32 unk_0C[3];
    VecFx32 unk_18[3];
    BOOL unk_3C;
} UnkStruct_ov115_022615B0;

typedef struct {
    UnkStruct_ov115_022615B0 unk_00[4];
} UnkStruct_ov115_02261574;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    UnkStruct_ov115_02261304 unk_08;
} UnkStruct_ov115_02261ADC_sub1;

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[3];
    fx32 unk_1D4;
    u8 unk_1D8;
    u8 unk_1D9;
    s16 unk_1DA;
    fx32 unk_1DC;
    fx32 unk_1E0;
} UnkStruct_ov115_02264FA0;

typedef struct {
    u16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov115_022653CC;

typedef struct {
    UnkStruct_ov115_02262DC4 unk_00;
    UnkStruct_ov115_02262DC4 unk_18;
    UnkStruct_ov115_02262DC4 unk_30;
    s32 unk_48;
} UnkStruct_ov115_02262E64;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    u8 unk_38;
    u8 unk_39;
    u8 unk_3A;
    u8 unk_3B;
    s32 unk_3C;
} UnkStruct_ov115_02263130;

typedef struct {
    u32 unk_00[4];
    u32 unk_10;
    u32 unk_14;
} UnkStruct_ov115_022632C8;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Window unk_38;
    TextOBJ *unk_48;
    UnkStruct_02012B20 *unk_4C;
    UnkStruct_02021AC8 unk_50;
    String *unk_5C;
    SpriteResource *unk_60;
    UnkStruct_ov115_02261304 unk_64;
    UnkStruct_ov115_02261304 unk_7C;
    s16 unk_94;
    s16 unk_96;
    u16 unk_98;
    s16 unk_9A;
    enum HeapID heapID;
} UnkStruct_ov115_02265788;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov115_022658E0;

typedef struct {
    BgConfig *unk_00;
    MessageFormat *unk_04;
    MsgData *unk_08;
    String *unk_0C;
    String *unk_10;
    u16 unk_14;
    u16 unk_16;
    GF3DVramMan *unk_18;
    SpriteList *unk_1C;
    G2dRenderer unk_20;
    GF_2DGfxResMan *unk_1AC[4];
    SpriteResourcesHeader unk_1BC;
    SpriteResource *unk_1E0[4];
    UnkStruct_02012744 *unk_1F0;
    Camera *camera;
    VecFx32 unk_1F8;
    UnkStruct_ov115_02264FA0 unk_204;
    UnkStruct_ov115_02261574 unk_3E8;
    UnkStruct_ov115_022617E8 unk_4E8;
    UnkStruct_ov115_02261ADC_sub1 unk_504;
    UnkStruct_ov115_02263DF8 unk_524;
    UnkStruct_ov114_0225CAD4 *unk_754;
    UnkStruct_ov115_022653CC unk_758;
    UnkStruct_ov115_02264A40 unk_760;
    UnkStruct_ov115_022647A0 unk_7F4[96];
    UnkStruct_ov115_02264BA0 unk_6A34;
    UnkStruct_ov115_02264C90 unk_6A3C;
    UnkStruct_ov115_02265788 unk_6B1C;
    NNSFndAllocator unk_6BBC;
} UnkStruct_ov115_02261ADC;

struct UnkStruct_ov115_02260D78_t {
    u32 unk_00;
    BOOL unk_04;
    s32 unk_08;
    s32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    UnkStruct_ov115_02262CA4 unk_20;
    UnkStruct_ov115_022622F8 unk_4C[96];
    UnkStruct_ov115_02261C18 unk_19CC;
    UnkStruct_ov115_02261ADC unk_1AB4;
    UnkStruct_ov115_02265AD4 unk_8680;
    TouchPadDataBuffer unk_8690;
    UnkStruct_ov115_02262E64 unk_86D4;
    const UnkStruct_ov115_02262F50 *unk_8720;
    u16 unk_8724;
    u16 unk_8726;
    UnkStruct_ov115_022612BC unk_8728;
    BOOL unk_872C;
    UnkStruct_ov115_02263130 unk_8730;
    UnkStruct_ov115_022632C8 unk_8770;
    UnkStruct_ov115_022658E0 unk_8788;
};

struct UnkStruct_02012744_t {
    void *unk_00[12];
    NNSG2dCellDataBank *unk_30[12];
    TextOBJ *unk_60;
    int unk_64;
};

void ov91_0225DD50(UnkStruct_ov115_02261C18 *param0, u16 param1);

BOOL ov91_0225F25C(UnkStruct_ov115_02263130 *param0, UnkStruct_ov115_02260D78 *param1);

void ov91_022601F4(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1);

s32 ov91_0225DB18(UnkStruct_ov115_022617E8 *, UnkStruct_ov115_02261ADC *);

s32 ov91_0225DB44(UnkStruct_ov115_022617E8 *, UnkStruct_ov115_02261ADC *);

s32 ov91_0225DB5C(UnkStruct_ov115_022617E8 *, UnkStruct_ov115_02261ADC *);

s32 ov91_0225DB7C(UnkStruct_ov115_022617E8 *, UnkStruct_ov115_02261ADC *);

s32 ov91_0225DBAC(UnkStruct_ov115_022617E8 *, UnkStruct_ov115_02261ADC *);

s32 ov91_0225DBC0(UnkStruct_ov115_02261ADC *, UnkStruct_ov115_02261C18 *);

s32 ov91_0225DD20(UnkStruct_ov115_02261C18 *, s32);

s32 ov91_0225F23C(UnkStruct_ov115_02263130 *);

s32 ov91_0225F7A8(UnkStruct_ov115_02260D78 *, UnkStruct_ov115_02261ADC *);

s32 ov91_022601AC(UnkStruct_ov115_02263DF8 *, UnkStruct_ov115_02261C18 *, u32);

s32 ov91_02260218(UnkStruct_ov115_02263DF8 *, UnkStruct_ov115_02261C18 *);

s32 ov91_0226031C(UnkStruct_ov115_02263DF8 *, s32);

s32 ov91_0225CEB4(UnkStruct_ov115_02260D78 *param0, u32 param1);

s32 ov91_0225CEB4(UnkStruct_ov115_02260D78 *param0, u32 param1) {
    s32 v1;
    s32 v2;
    u32 v0;

    v2 = 1;
    if ((u32)param0->unk_1AB4.unk_4E8.unk_00 >= 4U) {
        if (param0->unk_8728.unk_01 != 0) {
            v1 = -0x78;
        } else {
            v1 = 0x78;
        }
        ov91_0225DD50(&param0->unk_19CC, (u16)(param0->unk_19CC.unk_02 + v1));
    }
    ov91_0225DBC0(&param0->unk_1AB4, &param0->unk_19CC);
    ov91_022601F4(&param0->unk_1AB4.unk_524, &param0->unk_19CC);
    ov91_02260218(&param0->unk_1AB4.unk_524, &param0->unk_19CC);
    v0 = param0->unk_1AB4.unk_4E8.unk_00;
    switch (v0) {
    case 0:
        ov91_0225F23C(&param0->unk_8730);
        param0->unk_1AB4.unk_4E8.unk_00 += 1;
        break;
    case 1:
        if (ov91_0225F25C(&param0->unk_8730, param0) == 1) {
            param0->unk_1AB4.unk_4E8.unk_00 += 1;
            sub_02037AC0(2U);
        }
        break;
    case 2:
        if (sub_02037B38(2U) != 0) {
            param0->unk_1AB4.unk_4E8.unk_00 += 1;
            param0->unk_1AB4.unk_4E8.unk_08 = 0x41;
        }
        break;
    case 3:
        param0->unk_1AB4.unk_4E8.unk_08 -= 1;
        if (param0->unk_1AB4.unk_4E8.unk_08 == 0x32) {
            PlaySE(0x619U);
        }
        if (param0->unk_1AB4.unk_4E8.unk_08 == 0x14) {
            PlaySE(0x63DU);
        }
        ov91_0226031C(&param0->unk_1AB4.unk_524, 0x1000);
        if (param0->unk_1AB4.unk_4E8.unk_08 == 0) {
            ov91_0225DB7C(&param0->unk_1AB4.unk_4E8, &param0->unk_1AB4);
            param0->unk_1AB4.unk_4E8.unk_00 = 4;
        }
        break;
    case 4:
        if (param1 == 1) {
            param0->unk_1AB4.unk_4E8.unk_00 = 5;
        }
        break;
    case 5:
        ov91_0225DB18(&param0->unk_1AB4.unk_4E8, &param0->unk_1AB4);
        PlaySE(0x5DDU);
        param0->unk_1AB4.unk_4E8.unk_00 = 6;
        break;
    case 6:
        if (ov91_0225DB44(&param0->unk_1AB4.unk_4E8, &param0->unk_1AB4) == 0) {
            param0->unk_1AB4.unk_4E8.unk_00 = 7;
        }
        break;
    case 7:
        if (ov91_0225DBAC(&param0->unk_1AB4.unk_4E8, &param0->unk_1AB4) == 0) {
            param0->unk_1AB4.unk_4E8.unk_00 = 8;
        }
        break;
    case 8:
        ov91_0225DB5C(&param0->unk_1AB4.unk_4E8, &param0->unk_1AB4);
        ov91_0225DD20(&param0->unk_19CC, 1);
        v2 = 0;
        break;
    }
    ov91_022601AC(&param0->unk_1AB4.unk_524, &param0->unk_19CC, param0->unk_00);
    ov91_0225F7A8(param0, &param0->unk_1AB4);
    return v2;
}
