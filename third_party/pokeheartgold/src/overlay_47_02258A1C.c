#include "global.h"
struct UnkStruct_ov68_0225C91C;

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "player_data.h"
#include "vram_transfer_manager.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"

s32 ov47_02258AA0(void *, Options *, enum HeapID);

s32 ov47_02258BB4(void *, enum HeapID);

extern GraphicsBanks ov47_02259EC0;

void ov47_02258A1C(struct UnkStruct_ov68_0225C91C *arg0, SaveData *arg1, enum HeapID arg2);

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
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
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct WiFiPlazaNewsGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *spriteResources[4];
    NARC *narc;
} WiFiPlazaNewsGraphics;
void ov47_02258B8C(WiFiPlazaNewsGraphics *graphics);
void ov47_02258C44(WiFiPlazaNewsGraphics *graphics);
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;
void SpriteList_RenderAndAnimateSprites(UnkStruct_ov66_02234548 *param0);
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
struct UnkStruct_ov70_0225CA20_t {
    UnkStruct_ov63_0222BE18 *unk_00;
    UnkStruct_ov66_02234548 *unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    UnkStruct_ov63_0222D848 *unk_10;
    const UnkStruct_ov70_0225C894 *unk_14;
    UnkStruct_ov70_0225D93C unk_18;
    UnkStruct_ov70_0225CC54 *unk_20;
    UnkStruct_ov70_0225CC54 *unk_24;
};
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    BgConfig *unk_00;
    SpriteList *unk_04;
    G2dRenderer unk_08;
    SpriteResourceCollection *unk_194[4];
    G3DPipelineBuffers *unk_1A4;
    NARC *unk_1A8;
} UnkStruct_ov69_0225D35C;
void ov47_02258A60(WiFiPlazaNewsGraphics *graphics);
void ov47_02258A80(UnkStruct_ov70_0225CA20 *param0);
void ov47_02258A8C(UnkStruct_ov69_0225D35C *param0);

void ov47_02258A1C(UnkStruct_ov68_0225C91C *arg0, SaveData *saveData, enum HeapID heapID) {
    Options *options;

    options = Save_PlayerData_GetOptionsAddr(saveData);
    arg0->unk_1A4 = NARC_New(NARC_a_2_0_7, heapID);
    GF_CreateVramTransferManager(0x20U, heapID);
    GfGfx_SetBanks(&ov47_02259EC0);
    ov47_02258AA0(arg0, options, heapID);
    ov47_02258BB4(arg0, heapID);
}

void ov47_02258A60(WiFiPlazaNewsGraphics *graphics) {
    NARC_Delete(graphics->narc);
    GF_DestroyVramTransferManager();

    ov47_02258B8C(graphics);
    ov47_02258C44(graphics);
}

void ov47_02258A80(UnkStruct_ov70_0225CA20 *param0) {
    SpriteList_RenderAndAnimateSprites(param0->unk_04);
}

void ov47_02258A8C(UnkStruct_ov69_0225D35C *param0) {
    DoScheduledBgGpuUpdates(param0->unk_00);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}
