#include "global.h"
#include "sprite_resources_header.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"

struct BgConfig;
struct FieldEffectManager;
struct FontOAM;
struct G3DPipelineBuffers;
struct MapObject;
struct MessageLoader;
struct SpriteList;
struct SpriteResourceCollection;
struct StringTemplate;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_ov114_02025CAD4_t;
typedef struct MapObject MapObject;
void RemoveWindow(Simple3DModel *model);
typedef struct FieldEffectManager FieldEffectManager;
typedef struct {
    int unk_00;
    int unk_04;
    FieldEffectManager *unk_08;
    Simple3DModel unk_0C;
    Simple3DRenderObj unk_20;
    MapObject *unk_74;
} UnkStruct_021F50E8;
typedef struct FontOAM FontOAM;
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
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
typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;
void ov90_02258C8C(UnkStruct_ov114_0225CAD4 *param0);
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
void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);
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
BOOL ov90_02258CE0(const UnkStruct_ov114_0225CAD4 *param0);
void ClearWindowTilemapAndScheduleTransfer(Window *window);
void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);
void sub_02018198(Field3DModelAnimation *anim, fx32 frame);
void ov91_02260298(UnkStruct_ov115_02263DF8 *param0, u32 param1);
void ov91_02260334(UnkStruct_ov115_02263DF8 *param0, u16 param1);
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    fx32 unk_08;
    u8 pad_C[36];
    u8 pad_30[36];
    u8 pad_54[36];
    VecFx32 unk_78;
    VecFx32 unk_84;
    fx32 unk_90;
    fx32 unk_94;
    fx32 unk_98;
    VecFx32 unk_9C;
    fx32 unk_A8;
    fx32 unk_AC;
    fx32 unk_B0;
    fx32 unk_B4;
    fx32 unk_B8;
    fx32 unk_BC;
    fx32 unk_C0;
    fx32 unk_C4;
    fx32 unk_C8;
    fx32 unk_CC;
    fx32 unk_D0;
    fx32 unk_D4;
    VecFx32 unk_D8;
    fx32 unk_E4;
} UnkStruct_ov115_02261C18;
void ov91_022601C8(UnkStruct_ov115_02263DF8 *param0, const UnkStruct_ov115_02261C18 *param1);

void ov91_0225DB0C(UnkStruct_021F50E8 *param0);
void ov91_0225DB18(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1);
BOOL ov91_0225DB44(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1);
void ov91_0225DB5C(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1);
void ov91_0225DB7C(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1);
BOOL ov91_0225DBAC(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1);
void ov91_0225DBC0(UnkStruct_ov115_02261ADC *param0, const UnkStruct_ov115_02261C18 *param1);

void ov91_0225DB0C(UnkStruct_021F50E8 *param0) {
    RemoveWindow(&param0->unk_0C);
}

void ov91_0225DB18(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1) {
    DrawFrameAndWindow1(&param0->unk_0C, 0, 1, 4);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
    ov90_02258C8C(param1->unk_754);
}

BOOL ov91_0225DB44(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1) {
    BOOL v0 = ov90_02258CE0(param1->unk_754);

    if (v0 == 1) {
        return 0;
    }

    return 1;
}

void ov91_0225DB5C(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1) {
    ClearWindowPixelBuffer(&param0->unk_0C, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_0C);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
}

void ov91_0225DB7C(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1) {
    ov91_02260298(&param1->unk_524, 1);
    sub_02018198(&param1->unk_524.unk_198[1], ((fx32)(((180) > 0) ? ((180) * (1 << 12) + 0.5f) : ((180) * (1 << 12) - 0.5f))));
    ov91_02260334(&param1->unk_524, 1);
}

BOOL ov91_0225DBAC(UnkStruct_ov115_022617E8 *param0, UnkStruct_ov115_02261ADC *param1) {
    param0->unk_04++;

    if (param0->unk_04 >= 8) {
        return 0;
    }

    return 1;
}

void ov91_0225DBC0(UnkStruct_ov115_02261ADC *param0, const UnkStruct_ov115_02261C18 *param1) {
    ov91_022601C8(&param0->unk_524, param1);
}
