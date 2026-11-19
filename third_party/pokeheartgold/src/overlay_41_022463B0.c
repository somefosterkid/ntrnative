#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "sys_task.h"
#include "unk_0200B150.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"

s32 ov41_02246B68(void *, void *);

s32 ov41_02246BEC(void *, void *);

s32 ov41_02246D54(void *, s32, s32, s32);

void ov41_022463B0(void *arg0, void *arg1);

struct PokemonSpriteManager;
struct Resource;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
void ov41_02246DA8(UnkStruct_ov22_02255CB8 *param0);
typedef struct Resource Resource;
typedef struct ResourceCollection ResourceCollection;
Resource *GF2dGfxRawResMan_AllocObj(ResourceCollection *collection, void *data, int id);
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct PokemonSpriteManager PokemonSpriteManager;
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
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    int unk_08;
    int unk_0C;
    SoftwareSpritePalette **unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    PokemonSpriteManager *unk_20;
    NNSGfdTexKey unk_24;
    NNSGfdPlttKey unk_28;
    BOOL unk_2C;
    u8 *unk_30;
    ResourceCollection *unk_34;
    NNSG2dCharacterData **unk_38;
    int unk_3C;
    BgConfig *unk_40;
    SpriteList *unk_44;
    SpriteResourceCollection *unk_48[4];
    G2dRenderer unk_58;
    NARC *unk_5C;
    u8 pad_184[8];
    u32 unk_18C;
    u8 pad_190[164];
    u32 unk_234;
    u8 pad_238[40];
    u32 unk_260;
    u32 unk_264;
    u8 pad_268[252];
    u32 unk_364;
    u8 pad_368[840];
    u32 unk_6B0;
    u8 pad_6B4[4];
    u32 unk_6B8;
    u32 unk_6BC;
    u32 unk_6C0;
    u8 pad_6C4[24];
    u32 unk_6DC;
    u8 pad_6E0[8];
    u32 unk_6E8;
    u32 unk_6EC;
} UnkStruct_ov22_0225A0E4;
void PokepicManager_HandleLoadImgAndOrPltt(PokemonSpriteManager *monSpriteMan);
void ov41_022463D4(UnkStruct_ov22_02255CB8 *param0);
NNSG2dCharacterData *ov41_022463DC(UnkStruct_ov22_0225A0E4 *param0, void *param1, int param2);
void ov41_022463FC(void);
void ov41_0224642C(void);
void ov41_02246494(UnkStruct_ov22_0225A0E4 *param0);

void ov41_022463B0(void *arg0, void *arg1) {
    ov41_02246D54(arg1, 0x76, 0x13, 0xE);
    ov41_02246B68(arg0, arg1);
    ov41_02246BEC(arg0, arg1);
}

void ov41_022463D4(UnkStruct_ov22_02255CB8 *param0) {
    ov41_02246DA8(param0);
}

NNSG2dCharacterData *ov41_022463DC(UnkStruct_ov22_0225A0E4 *param0, void *param1, int param2) {
    GF2dGfxRawResMan_AllocObj(param0->unk_34, param1, param2);
    NNS_G2dGetUnpackedCharacterData(param1, &param0->unk_38[param2]);

    return param0->unk_38[param2];
}

void ov41_022463FC(void) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
}

void ov41_0224642C(void) {
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 0);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ, 0);
    G2_SetWnd0Position(8 + 2, 16 + 2, (136 + 2) + (112 - (2 * 2)), (16 + 2) + (129 - (2 * 2)));
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
}

void ov41_02246494(UnkStruct_ov22_0225A0E4 *param0) {
    DoScheduledBgGpuUpdates(param0->unk_40);
    PokepicManager_HandleLoadImgAndOrPltt(param0->unk_20);
    OamManager_ApplyAndResetBuffers();
}
