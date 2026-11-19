#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"

struct FontOAM;
struct G3DPipelineBuffers;
struct MessageLoader;
struct SpriteList;
struct SpriteResourceCollection;
struct StringTemplate;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_ov114_02025CAD4_t;
typedef struct FontOAM FontOAM;
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
typedef struct SpriteList SpriteList;
typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;
struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};
void Sprite_Delete(Sprite *gfxElem);
typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;
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
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct MessageLoader MessageLoader;
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct StringTemplate StringTemplate;
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
    Easy3DObject unk_00[3];
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
    Easy3DObject unk_04;
    Easy3DObject unk_7C;
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
    Easy3DObject unk_00[3];
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
    FontOAM *unk_48;
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
    StringTemplate *unk_04;
    MessageLoader *unk_08;
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
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
void Sprite_SetPaletteOverride(Sprite *sprite, u32 palette);
BOOL ov91_02260A88(UnkStruct_ov115_022647A0 *param0, const UnkStruct_ov115_022622F8 *param1, u32 param2);
void ov91_02260AF8(UnkStruct_ov115_022647A0 *param0, UnkStruct_ov115_02261ADC *param1);
void sub_020181EC(Easy3DObject *obj);
BOOL sub_020182A4(const Easy3DObject *obj);
void sub_020182A8(Easy3DObject *obj, fx32 x, fx32 y, fx32 z);
void ov91_02260950(UnkStruct_ov115_022647A0 *param0);
void ov91_02260A10(UnkStruct_ov115_022647A0 *param0);
Field3dModel *ov91_02260B48(const UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02264A40 *param1);
void ov91_02260B5C(UnkStruct_ov115_022647A0 *param0);

void ov91_02260830(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1);
void ov91_02260848(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1, const UnkStruct_ov115_022622F8 *param2, u32 param3);
void ov91_02260884(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1, const UnkStruct_ov115_022622F8 *param2);
void ov91_022608A8(UnkStruct_ov115_022647A0 *param0, UnkStruct_ov115_02261ADC *param1, u32 param2);

void ov91_02260830(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1) {
    Sprite_Delete(param1->unk_00);
    memset(param1, 0, sizeof(UnkStruct_ov115_022647A0));
}

void ov91_02260848(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1, const UnkStruct_ov115_022622F8 *param2, u32 param3) {
    param1->unk_F8 = param2;
    param1->unk_F4 = 1;
    param1->unk_104 = 0;

    Sprite_SetPaletteOverride(param1->unk_00, param2->unk_04.unk_00);
    ov91_02260A88(param1, param2, param3);
    Sprite_SetDrawFlag(param1->unk_00, 1);
}

void ov91_02260884(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_022647A0 *param1, const UnkStruct_ov115_022622F8 *param2) {
    param1->unk_F8 = param2;
    param1->unk_104 = 0;

    Sprite_SetDrawFlag(param1->unk_00, 0);
    ov91_02260AF8(param1, param0);
}

void ov91_022608A8(UnkStruct_ov115_022647A0 *param0, UnkStruct_ov115_02261ADC *param1, u32 param2) {
    BOOL v0;
    Field3dModel *v1;

    if (param0->unk_F4 == 1) {
        v0 = ov91_02260A88(param0, param0->unk_F8, param2);

        if (v0 == 0) {
            ov91_02260AF8(param0, param1);
        }
    }

    if (param0->unk_F4 == 0) {
        v1 = ov91_02260B48(param0->unk_F8, &param1->unk_760);

        if (param0->unk_F8->unk_00 == 5) {
            NNS_G3dMdlSetMdlAmbAll(v1->model, ((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))));
        } else {
            NNS_G3dMdlSetMdlAmbAll(v1->model, ((GXRgb)(((18) << (0)) | ((18) << (5)) | ((18) << (10)))));
        }

        ov91_02260B5C(param0);
        sub_020182A8(&param0->unk_04, param0->unk_F8->unk_2C.x, param0->unk_F8->unk_2C.y, param0->unk_F8->unk_2C.z);
        sub_020181EC(&param0->unk_04);

        if (sub_020182A4(&param0->unk_7C) == 1) {
            ov91_02260950(param0);
            sub_020181EC(&param0->unk_7C);
            ov91_02260A10(param0);
        }
    }
}
