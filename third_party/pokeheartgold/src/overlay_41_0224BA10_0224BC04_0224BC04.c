#include "global.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "gf_3d_loader.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct BgConfig;

struct DressUpPhoto;

struct MailMessage;

struct NARC;

struct PhotoAccessory;

struct PhotoPokemon;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SpriteAnimFrame;

struct UnkStruct_02029C88_t;

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

typedef struct DressUpPhoto {
    u32 integrity;
    PhotoPokemon photoMon;
    u32 unk_3C;
    MailMessage title;
    PhotoAccessory accessories[10];
    u8 unk_70;
    u8 language;
} DressUpPhoto;

typedef struct UnkStruct_02029C88_t {
    u32 integrity;
    u32 unk_04;
    PhotoPokemon photoMon;
    u32 unk_40;
    PhotoAccessory accessories[20];
    u8 unk_94;
} UnkStruct_02029C88;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

void LoadRectToBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, const void *src, u8 destX, u8 destY, u8 destWidth, u8 destHeight);

void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, u32 narcMemberIdx, enum GFPalLoadLocation loadLocation, u32 offset, u32 size, u32 heapID);

void *GfGfxLoader_GetScrnData(u32 narcID, u32 narcMemberIdx, BOOL compressed, NNSG2dScreenData **outScreenData, u32 heapID);

void Heap_Free(void *ptr);

void ov41_0224BC04(UnkStruct_ov22_0225B85C *param0);

void ov41_0224BC04(UnkStruct_ov22_0225B85C *param0) {
    void *v0;
    NNSG2dScreenData *v1;
    GfGfxLoader_GXLoadPalFromOpenNarc(param0->unk_14.unk_5C, 126, (enum GFPalLoadLocation)0, 3 * 32, 64, HEAP_ID_14);
    GfGfxLoader_LoadCharDataFromOpenNarc(param0->unk_14.unk_5C, 125, param0->unk_14.unk_40, 1, 0, 0, 0, HEAP_ID_14);
    v0 = GfGfxLoader_GetScrnData(26, 128, 0, &v1, HEAP_ID_14);
    LoadRectToBgTilemapRect(param0->unk_14.unk_40, 1, v1->rawData, 0, 0, v1->screenWidth / 8, v1->screenHeight / 8);
    BgTilemapRectChangePalette(param0->unk_14.unk_40, 1, 0, 0, v1->screenWidth / 8, v1->screenHeight / 8, 4);
    Heap_Free(v0);
    ScheduleBgTilemapBufferTransfer(param0->unk_14.unk_40, 1);
}
