#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "field/field_3d_model.h"

struct Background;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct TextOBJ;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_ov114_02025CAD4_t;

struct Window;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

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

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;

Camera *Camera_New(const enum HeapID heapID);

void Camera_SetStaticPtr(Camera *camera);

void Camera_SetLookAtCamUp(const VecFx32 *up, Camera *camera);

void Camera_SetPerspectiveClippingPlane(const fx32 nearClip, const fx32 farClip, Camera *camera);

void Camera_Init_FromTargetDistanceAndAngle(const VecFx32 *target, const fx32 distance, const CameraAngle *angle, const u16 fovY, const u8 projection, const BOOL trackTarget, Camera *camera);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

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
    BgConfig *unk_00;
    MessageFormat *unk_04;
    struct MsgData *unk_08;
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

extern const u16 ov91_02261D64[4][4];

void ov91_0225FD2C(UnkStruct_ov115_02261ADC *param0, u32 param1, u32 param2, u32 heapID);

void ov91_0225FD2C(UnkStruct_ov115_02261ADC *param0, u32 param1, u32 param2, u32 heapID) {
    CameraAngle v0;
    VecFx32 v1;

    param0->camera = Camera_New((const enum HeapID)(heapID));
    param0->unk_1F8.x = 0;
    param0->unk_1F8.y = 0;
    param0->unk_1F8.z = 0;

    v0.x = 0xf112;
    v0.y = ov91_02261D64[param1 - 1][param2];
    v0.z = ((0 * 0xffff) / 360);

    Camera_Init_FromTargetDistanceAndAngle(&param0->unk_1F8, 0x1d9000, &v0, (22 * 0xffff) / 360, 0, 1, param0->camera);

    v1.x = 0;
    v1.y = ((fx32)0x0000000000001000L);
    v1.z = 0;

    Camera_SetLookAtCamUp(&v1, param0->camera);
    Camera_SetStaticPtr(param0->camera);
    Camera_SetPerspectiveClippingPlane(((fx32)(((200) > 0) ? ((200) * (1 << 12) + 0.5f) : ((200) * (1 << 12) - 0.5f))), ((fx32)(((1000) > 0) ? ((1000) * (1 << 12) + 0.5f) : ((1000) * (1 << 12) - 0.5f))), param0->camera);
}
