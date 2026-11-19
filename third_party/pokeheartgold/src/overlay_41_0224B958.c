#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_0225a0e4_head.h"

struct PokemonSprite;
struct PokemonSpriteTransforms;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
typedef struct PhotoAccessory {
    u8 unk_00;
    u8 xPos;
    u8 yPos;
    s8 priority;
} PhotoAccessory;
typedef struct PhotoPokemon {
    u32 personality;
    u32 otID;
    u16 species;
    u16 nickname[10 + 1];
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
    u8 trainerGender;
} PhotoPokemon;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SoftwareSpriteCharsTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dCharacterData *charsData;
} SoftwareSpriteCharsTemplate;
typedef struct SoftwareSpritePaletteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dPaletteData *paletteData;
    u32 paletteSlot;
} SoftwareSpritePaletteTemplate;
GF_2DGfxRawResObj *GF2dGfxRawResMan_AllocObj(GF_2DGfxRawResMan *collection, void *data, int id);
typedef struct {
    SoftwareSpriteCharsTemplate *unk_00;
    int unk_04;
    SoftwareSpritePaletteTemplate *unk_08;
    int unk_0C;
    GF_2DGfxRawResMan *unk_10;
    GF_2DGfxRawResMan *unk_14;
} UnkStruct_ov22_02255CB8;
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);
struct PokemonSpriteTransforms {
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
typedef struct PokemonSpriteShadow {
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
} PokemonSpriteShadow;
struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    PokepicAnimScript animFrames[10];
};
typedef struct PokemonSpriteManager {
    PokemonSprite sprites[4];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    enum HeapID heapID;
    u32 charBaseAddr;
    u32 charSize;
    u32 plttBaseAddr;
    u32 plttSize;
    u8 *charRawData;
    u16 *plttRawData;
    u16 *plttRawDataUnfaded;
    NNSG2dCharacterData charData;
    NNSG2dPaletteData plttData;
    u8 dummy330;
    u8 needLoadChar;
    u8 needLoadPltt;
    u8 excludeIdentity;
    u32 hideShadows;
} PokemonSpriteManager;
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
u32 sub_0202BEFC(const PhotoAccessory *accessory);
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

void ov41_0224B958(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID);

void ov41_0224B958(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID) {
    int v0;
    int v1;
    void *v2;
    int v3;

    for (v0 = 0; v0 < param2->unk_54; v0++) {
        v3 = sub_0202BEFC(param2->unk_04[v0]);
        v1 = v3;

        if (GF2dGfxRawResMan_DoesNotHaveObjWithId(param1->unk_10, v1) == 1) {
            v2 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, v3 + 1, 0, heapID, 1);

            GF2dGfxRawResMan_AllocObj(param1->unk_10, v2, v1);
            NNS_G2dGetUnpackedCharacterData(v2, &param1->unk_00[v1].charsData);

            param1->unk_00[v1].softSpriteMan = param0->unk_00;
        }
    }

    v2 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, 0, 0, heapID, 1);

    GF2dGfxRawResMan_AllocObj(param1->unk_14, v2, 0);
    NNS_G2dGetUnpackedPaletteData(v2, &param1->unk_08[0].paletteData);

    param1->unk_08[0].softSpriteMan = param0->unk_00;
    param1->unk_08[0].paletteSlot = 3;
}
