#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BgConfig;

struct NARC;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct SysTask;

struct SysTaskManager;

struct Window;

typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;

typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;

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
    int xScale;
    int yScale;
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

void CopyWindowToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

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

typedef struct SpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} SpriteListTemplate;

Sprite *Sprite_Create(const SpriteListTemplate *template);

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

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_0225AD68;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04[2];
    VecFx32 unk_0C[2];
    UnkStruct_ov22_0225AD68 unk_24;
    UnkStruct_ov22_0225AD68 unk_38;
    UnkStruct_ov22_0225AD68 unk_4C;
} UnkStruct_ov22_0225AE9C;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[2];
    Window *unk_18;
    int unk_1C;
    int unk_20;
    SysTask *unk_24;
    SysTask *unk_28;
    UnkStruct_02095C60 *unk_2C;
    UnkStruct_ov22_0225AE9C unk_30;
    BOOL unk_90;
} UnkStruct_ov22_0225AB54;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

typedef u32 TextColor;

void ov22_0225A748(Window **param0, BgConfig *param1, int param2, int param3, int param4, int param5, int param6, BOOL param7);

s32 ov22_0225A860(Window *param0, enum NarcId narcID, int param2, int param3, int param4, int param5, TextColor param6, u32 param7);

void ov22_0225ACE4(UnkStruct_ov22_0225AB54 *param0);

void ov22_0225AA34(SpriteResource **param0, SpriteResourceCollection **param1, int param2, NARC *param3, int param4, int param5, int param6, int param7, int param8, int param9);

void ov22_0225AAF4(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3);

u8 ov41_0224AE78(Window *param0, enum NarcId narcID, int bankID, int param3, int param4, int param5, u32 param6, u32 param7, String **param8);

void ov41_0224B118(UnkStruct_ov22_0225AB54 *param0, SpriteList *param1, SpriteResourceCollection **param2, int param3, BgConfig *param4, UnkStruct_02095C60 *param5, NARC *param6);

void ov41_0224AFF8(SpriteResource **param0, SpriteResourceCollection **param1, int param2, NARC *param3, int param4, int param5, int param6, int param7, int param8, int param9);

void ov41_0224B0B8(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3);

void ov41_0224B298(UnkStruct_ov22_0225AB54 *param0);

void ov41_0224AD0C(Window **param0, BgConfig *param1, int param2, int param3, int param4, int param5, int param6, BOOL param7);

u8 ov41_0224AE24(Window *param0, enum NarcId narcID, int param2, int param3, int param4, int param5, TextColor param6, u32 param7);

void ov41_0224B118(UnkStruct_ov22_0225AB54 *param0, SpriteList *param1, SpriteResourceCollection **param2, int param3, BgConfig *param4, UnkStruct_02095C60 *param5, NARC *param6) {
    SpriteResourcesHeader v0;
    SpriteListTemplate v1;
    unsigned char new_var;
    int v2;
    ov41_0224AFF8(param0->unk_00, param2, 14, param6, 229, 230, 228, 227, 2, 3000);
    ov41_0224B0B8(param0->unk_00, param2, &v0, 0);
    v1.list = param1;
    v1.resourceData = &v0;
    v1.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    v1.priority = 0;
    v1.heapID = HEAP_ID_14;
    v1.position.y = 58 * ((fx32)0x0000000000001000L);
    v1.position.y += 512 * ((fx32)0x0000000000001000L);
    for (v2 = 0; v2 < 2; v2++) {
        v1.position.x = 103 + (v2 * 24);
        v1.position.x *= (fx32)0x0000000000001000L;
        param0->unk_10[v2] = Sprite_Create(&v1);
    }

    param0->unk_1C = param3;
    param0->unk_20 = param3 * 30;
    param0->unk_2C = param5;
    param0->unk_2C->unk_00.unk_00 = param3;
    param0->unk_2C->unk_08.unk_00 = param3;
    param0->unk_90 = 0;
    ov41_0224B298(param0);
    ov41_0224AD0C(&param0->unk_18, param4, 10, 8, 14, 4, (18 + 12) + ((1 + (27 * 4)) + (27 * 2)), 0);
    FillWindowPixelBuffer(param0->unk_18, 0);
    new_var = 215;
    ov41_0224AE24(param0->unk_18, (enum NarcId)27, new_var, 4, 0, 4, (TextColor)((((1 & 0x1B) << 16) | ((2 & 0x1B) << 8)) | ((0 & 0x1B) << 0)), 0xff);
    ov41_0224AE24(param0->unk_18, (enum NarcId)27, new_var, 5, 0 + 72, 4, (TextColor)(((0 & 0x1B) << 0) | (((1 & 0x1B) << 16) | ((2 & 0x1B) << 8))), 0xff);
    CopyWindowToVram(param0->unk_18);
}
