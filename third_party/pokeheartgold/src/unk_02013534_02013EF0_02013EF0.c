#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "screen_fade_internal.h"

struct Background;

struct BgConfig;

struct TextOBJ;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResourcesHeader;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_02013034_t;

struct Window;

typedef struct TextOBJ TextOBJ;

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

typedef struct SpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} SpriteListTemplate;

Sprite *Sprite_Create(const SpriteListTemplate *template);

const VecFx32 *Sprite_GetMatrixPtr(const Sprite *sprite);

typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;

typedef struct {
    Sprite *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02012CE0_sub1;

struct TextOBJ {
    UnkStruct_02012CE0_sub1 *unk_00;
    int unk_04;
    const Sprite *unk_08;
    int x;
    int y;
};

struct UnkStruct_02012744_t {
    void *unk_00[12];
    NNSG2dCellDataBank *unk_30[12];
    TextOBJ *unk_60;
    int unk_64;
};

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_02012DE4;

typedef struct {
    UnkStruct_02012DE4 unk_00;
    UnkStruct_02012DE4 unk_10;
    u8 unk_20;
} UnkStruct_02012D34;

typedef struct UnkStruct_02013034_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_02013034_t *unk_0C;
    struct UnkStruct_02013034_t *unk_10;
} UnkStruct_02013034;

struct UnkStruct_02012B20_t {
    UnkStruct_02013034 unk_00;
    int unk_14;
};

UnkStruct_02012744 *FontSystem_NewInit(int param0, enum HeapID heapID);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

UnkStruct_02012B20 *sub_02012B20(const Window *param0, enum HeapID heapID);

void TextOBJ_CopyFromBGWindow(TextOBJ *param0, const UnkStruct_02012B20 *param1, const Window *param2, enum HeapID heapID);

BOOL sub_02012D34(UnkStruct_02012D34 *param0, UnkStruct_02013034 *param1, enum HeapID heapID);

int sub_02012DE4(int param0, int param1, enum HeapID heapID, UnkStruct_02013034 *param3);

void sub_02012E6C(void *arg0, void *arg1, s32 arg2, s32 arg3, enum GFScreen arg4, HardwareWindowSettings *arg5, s32 arg6, s32 arg7);

UnkStruct_02013034 *sub_02013188(enum HeapID heapID);

Sprite *sub_02013EF0(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2);

Sprite *sub_02013EF0(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2) {
    SpriteListTemplate v0;
    SpriteResourcesHeader v1;

    v1.imageProxy = param2;
    v1.charData = 0L;
    v1.plttProxy = param0->unk_0C;
    v1.cellData = param0->unk_00->unk_30[param1->unk_08];
    v1.cellAnim = 0L;
    v1.multiCellData = 0L;
    v1.multiCellAnim = 0L;
    v1.flag = 0;
    v1.priority = param0->unk_20;

    v0.list = param0->unk_08;
    v0.resourceData = &v1;
    v0.priority = param0->unk_24;
    v0.vramType = (NNS_G2D_VRAM_TYPE)(param0->unk_28);
    v0.heapID = param0->heapID;
    v0.position.x = 0;
    v0.position.y = 0;
    v0.position.z = 0;

    if (param0->unk_10) {
        const VecFx32 *v2;

        v2 = Sprite_GetMatrixPtr(param0->unk_10);
        v0.position = *v2;
    }

    v0.position.x += (param0->unk_18 << 12) + ((param1->unk_00 * 8) << 12);
    v0.position.y += (param0->unk_1C << 12) + ((param1->unk_04 * 8) << 12);

    return Sprite_Create(&v0);
}
