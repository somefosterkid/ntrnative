#include "global.h"

#include "gf_gfx_loader.h"
#include "bg_window.h"
#include "filesystem.h"


struct DressUpPhoto;
struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
struct UnkStruct_02029C88_t;
struct UnkStruct_ov22_0225B1BC_t;
typedef struct DressUpPhoto DressUpPhoto;
typedef struct UnkStruct_02029C88_t UnkStruct_02029C88;
typedef struct ResourceCollection ResourceCollection;
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
void Sprite_SetPriority(Sprite *sprite, u8 priority);
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
} UnkStruct_ov22_0225A0E4;
Sprite *ov41_02246280(UnkStruct_ov22_0225A0E4 *param0, int param1, int param2, int param3, int param4, int param5);
void ov41_022462E4(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4, int param5);
void ov41_02246304(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4, int param5, int param6);
void ov41_02246328(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4);
void ov41_02246344(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4);
typedef struct UnkStruct_ov22_0225B1BC_t UnkStruct_ov22_0225B1BC;
typedef struct {
    const DressUpPhoto *photo;
    const UnkStruct_02029C88 *unk_04;
    u32 unk_08;
    u32 unk_0C;
    UnkStruct_ov22_0225B1BC *unk_10;
    UnkStruct_ov22_0225A0E4 unk_14;
    Sprite *unk_1FC;
    Window *unk_200;
} UnkStruct_ov22_0225B85C;
void ov41_0224BCF0(UnkStruct_ov22_0225B85C *param0);
void ov41_0224BCA4(UnkStruct_ov22_0225B85C *arg0);

void ov41_0224BCA4(UnkStruct_ov22_0225B85C *arg0) {
    GfGfxLoader_GXLoadPal(NARC_a_2_3_7, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_14);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_7, 9, arg0->unk_14.unk_40, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_14);
    GfGfxLoader_LoadCharData(NARC_a_2_3_7, 1, arg0->unk_14.unk_40, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_14);
}

void ov41_0224BCF0(UnkStruct_ov22_0225B85C *param0) {
    ov41_022462E4(&param0->unk_14, param0->unk_14.unk_5C, 233, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    ov41_02246304(&param0->unk_14, param0->unk_14.unk_5C, 234, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 6, 1000);
    ov41_02246328(&param0->unk_14, param0->unk_14.unk_5C, 232, 0, 1000);
    ov41_02246344(&param0->unk_14, param0->unk_14.unk_5C, 231, 0, 1000);

    param0->unk_1FC = ov41_02246280(&param0->unk_14, 1000, 0, 144, 100, NNS_G2D_VRAM_TYPE_2DMAIN);

    Sprite_SetPriority(param0->unk_1FC, 1);
}
