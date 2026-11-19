#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"
#include "gf_3d_loader.h"
#include "filesystem_files_def.h"

struct BgConfig;

struct NARC;

struct PhotoAccessory;

struct PhotoPokemon;

struct Pokemon;

struct Pokepic;

struct PokepicManager;

struct PokepicDrawParam;

struct GF_2DGfxRawResMan;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteCharsTemplate;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SoftwareSpritePaletteTemplate;

struct SpriteList;

struct SpriteResourceCollection;


typedef struct PhotoAccessory PhotoAccessory;

typedef struct PhotoPokemon PhotoPokemon;


typedef struct BgConfig BgConfig;


typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SoftwareSpriteManagerTemplate {
    int numSprites;
    int numChars;
    int numPalettes;
    enum HeapID heapID;
} SoftwareSpriteManagerTemplate;

typedef struct SoftwareSpriteCharsTemplate SoftwareSpriteCharsTemplate;

typedef struct SoftwareSpritePaletteTemplate SoftwareSpritePaletteTemplate;





typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokepicDrawParam PokepicDrawParam;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

struct PokepicDrawParam {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef struct PokepicShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokepicShadow;

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokepicDrawParam transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokepicShadow shadow;
    PokepicShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

typedef struct PokemonSpriteManager PokepicManager;

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


void ov41_02246250(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1);

void ov41_022463D4(UnkStruct_ov22_02255CB8 *param0);

void ov41_02246518(UnkStruct_ov22_0225A0E4_Head *param0, const SoftwareSpriteManagerTemplate *param1, enum HeapID heapID);



typedef struct {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;


typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_020298D8 unk_4C;
} UnkStruct_ov22_02257964_sub2;




typedef struct {
    const PhotoPokemon *photoMon;
    const PhotoAccessory *unk_04[20];
    int unk_54;
    BgConfig *unk_58;
    int unk_5C;
    int unk_60;
    int unk_64;
    enum HeapID heapID;
} UnkStruct_ov22_0225B4E4;

void ov41_0224B938(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID);

void ov41_0224B6CC(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void ov41_022465D8(UnkStruct_ov22_0225A0E4_Head *param0, int param1, int param2, u16 param3, const VecFx32 *param4);
void ov41_0224B720(UnkStruct_ov22_0225B1BC *param0);

void ov41_0224B6CC(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1) {
    UnkStruct_ov22_02255CB8 v0;
    SoftwareSpriteManagerTemplate v1;

    param0->unk_00.unk_5C = NARC_New(NARC_a_0_2_6, param1->heapID);

    v1.numSprites = (700 + 18);
    v1.numChars = (100 + 18);
    v1.numPalettes = (1 + 18);
    v1.heapID = param1->heapID;

    ov41_02246518(&param0->unk_00, &v1, param1->heapID);
    ov41_0224B938(&param0->unk_00, &v0, param1, param1->heapID);
    ov41_02246250(&param0->unk_00, &v0);
    ov41_022463D4(&v0);
}

void ov41_0224B720(UnkStruct_ov22_0225B1BC *param0) {
    ov41_022465D8(&param0->unk_00, param0->unk_23C + param0->unk_244, param0->unk_240 + param0->unk_248, param0->unk_24C, &param0->unk_250);
}
