#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

s32 ov91_02260848(void *, void *, void *, s32);

s32 ov91_02260884(void *, void *, void *);

void *ov91_02260A50(void *arg0);

void ov91_02260728(void *arg0, void *arg1, s32 arg2, s32 arg3);

struct TextOBJ;
struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_ov114_02025CAD4_t;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
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
    BgConfig *unk_00;
    MessageFormat *unk_04;
    MsgData *unk_08;
    String *unk_0C;
    String *unk_10;
    u16 unk_14;
    u16 unk_16;
    G3DPipelineBuffers *unk_18;
    SpriteList *unk_1C;
    G2dRenderer unk_20;
    SpriteResourceCollection *unk_1AC[4];
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
void ov91_02260A2C(UnkStruct_ov115_022647A0 *param0);
void ov91_022608A8(UnkStruct_ov115_022647A0 *param0, UnkStruct_ov115_02261ADC *param1, u32 param2);
void ov91_02260754(UnkStruct_ov115_02261ADC *param0, const UnkStruct_ov115_022622F8 *param1);
void ov91_0226078C(UnkStruct_ov115_02261ADC *param0, u32 param1);

void ov91_02260728(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    void *v0;

    v0 = ov91_02260A50(arg0);
    if (*(u16 *)((u8 *)arg1 + 4) == arg3) {
        ov91_02260848(arg0, v0, arg1, arg2);
        return;
    }
    ov91_02260884(arg0, v0, arg1);
}

void ov91_02260754(UnkStruct_ov115_02261ADC *param0, const UnkStruct_ov115_022622F8 *param1) {
    int v0;

    for (v0 = 0; v0 < 96; v0++) {
        if (param0->unk_7F4[v0].unk_F8 == param1) {
            ov91_02260A2C(&param0->unk_7F4[v0]);
            return;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
}

void ov91_0226078C(UnkStruct_ov115_02261ADC *param0, u32 param1) {
    int v0;

    for (v0 = 0; v0 < 96; v0++) {
        if (param0->unk_7F4[v0].unk_F8 != 0L) {
            ov91_022608A8(&param0->unk_7F4[v0], param0, param1);
        }
    }
}
