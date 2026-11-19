#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "field/field_3d_model.h"

struct BgConfig;

struct Camera;

struct CharTransferAllocation;

struct TextOBJ;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct MsgData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct TouchPadDataBuffer;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov115_02260D78_t;

struct Window;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

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

typedef struct String String;

typedef struct PlayerProfile PlayerProfile;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;

typedef struct TextOBJ TextOBJ;

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

typedef struct Camera Camera;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct MsgData MsgData;

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct MessageFormat MessageFormat;

typedef struct TouchPadDataBuffer {
    u16 bufferSize;
    TPData buffer[8];
} TouchPadDataBuffer;

enum TouchPadExternalBufferWriteMethod {
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING = 1,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITH_WRAPPING,
};

u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold);

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
    CharTransferAllocation unk_50;
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

void ov91_0225DCC4(const UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_02261C18 *param1);

void ov91_0225E40C(UnkStruct_ov115_02260D78 *param0);

void ov91_0225E62C(UnkStruct_ov115_02260D78 *param0);

void ov91_0225EA7C(UnkStruct_ov115_02260D78 *param0);

BOOL ov91_0225EF70(UnkStruct_ov115_02262E64 *param0);

void ov91_0225F008(UnkStruct_ov115_02260D78 *param0, const UnkStruct_ov115_02262E64 *param1);

void ov91_0225F3D4(UnkStruct_ov115_022632C8 *param0);

u32 ov91_0225F3E4(const UnkStruct_ov115_022632C8 *param0);

BOOL ov91_0225F3E8(const UnkStruct_ov115_022632C8 *param0, u32 param1);

void ov91_0225F74C(const UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_02261ADC *param1);

void ov91_02260370(UnkStruct_ov115_02263DF8 *param0, u32 param1);

void ov91_022619E8(UnkStruct_ov115_022658E0 *param0);

void ov91_02261A00(UnkStruct_ov115_022658E0 *param0, UnkStruct_ov115_02264FA0 *param1, UnkStruct_ov115_02263DF8 *param2);

extern const u32 ov91_02261C44[3];

BOOL ov91_0225D1DC(UnkStruct_ov115_02260D78 *param0);

BOOL ov91_0225D1DC(UnkStruct_ov115_02260D78 *param0) {
    BOOL v0;

    if (param0->unk_00 < 3 - 1) {
        if (ov91_02261C44[(param0->unk_00 + 1) - 1] <= param0->unk_08) {
            param0->unk_00++;

            if (param0->unk_00 == 1) {
                ov91_022619E8(&param0->unk_8788);
            }
        }
    }

    sub_02021280(&param0->unk_8690, TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING, 0);
    ov91_0225EA7C(param0);

    {
        v0 = ov91_0225EF70(&param0->unk_86D4);

        if (v0 == 0) {
            ov91_0225F008(param0, &param0->unk_86D4);
        }
    }

    {
        u32 v1;

        v0 = ov91_0225F3E8(&param0->unk_8770, param0->unk_10);

        if (v0 == 1) {
            v1 = ov91_0225F3E4(&param0->unk_8770);

            if (v1 != 0xffff) {
                ov91_02260370(&param0->unk_1AB4.unk_524, v1);
            }

            ov91_0225F3D4(&param0->unk_8770);
        }
    }

    ov91_0225E40C(param0);
    ov91_0225DCC4(param0, &param0->unk_19CC);
    ov91_0225F74C(param0, &param0->unk_1AB4);
    ov91_0225E62C(param0);
    ov91_02261A00(&param0->unk_8788, &param0->unk_1AB4.unk_204, &param0->unk_1AB4.unk_524);

    if (param0->unk_04 == 1) {
        param0->unk_08++;
    }

    if (param0->unk_08 >= param0->unk_0C) {
        param0->unk_08 = param0->unk_0C;
        return 1;
    }

    return 0;
}
