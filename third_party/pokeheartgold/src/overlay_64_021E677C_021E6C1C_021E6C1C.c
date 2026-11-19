#include "global.h"

#include "constants/heap.h"

#include "dex_mon_measures.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"

typedef enum HeapID HeapID;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct ManagedSprite;

struct ManagedSpriteTemplate;

struct NARC;

struct PokedexData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct SpriteSystem;

struct UnkStruct_ov64_021E6C1C_Tmpl;

extern const s16 ov64_021E6ECC[];

extern const s16 ov64_021E6ECE[];

typedef struct UnkStruct_ov64_021E6C1C_Tmpl {
    s16 x;
    s16 y;
    u8 filler_04[0x10];
    int resId;
    int unk_18;
    int unk_1C;
    int unk_20;
    u8 filler_24[0x10];
} UnkStruct_ov64_021E6C1C_Tmpl;

extern const UnkStruct_ov64_021E6C1C_Tmpl ov64_021E6FD4;

s32 ov64_021E6E30(s32 species, s32 arg1, s32 arg2);

void ov64_021E5AAC(void *arg0, u32 offset, u32 arg2);

void ov64_021E5AE4(void *arg0, u32 offset, u32 arg2);

void ov64_021E6C1C(HallOfFameShowcase *work, int slot, int pos, int resId, int param5);

void ov64_021E6C1C(HallOfFameShowcase *work, int slot, int pos, int resId, int param5) {
    u8 hdr[4];
    UnkStruct_ov64_021E6C1C_Tmpl tmpl;
    NARC *narc;
    int fileId;
    int vramRes;
    int n;
    u32 loc;
    u32 size;
    u8 gender;
    u16 fileIdx;
    void *res;
    u8 *tex;
    u8 *texImg;
    void *buf;
    u32 palLoc;
    u8 *palData;

    ReadWholeNarcMemberByIdPair(hdr, (NarcId)(0x8d), SpeciesToOverworldModelIndexOffset(work->mon.species));
    narc = NARC_New((NarcId)(0x61), (HeapID)(0x3b));
    if (hdr[1] != 0) {
        fileId = 0xD;
        vramRes = 0xDCC2;
        n = 8;
    } else {
        fileId = 0xA;
        vramRes = 0xDCC1;
        n = 4;
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, fileId, 1, 1, resId);
    NARC_Delete(narc);

    tmpl = ov64_021E6FD4;
    tmpl.resId = resId;
    tmpl.unk_18 = param5;
    tmpl.unk_1C = vramRes;
    tmpl.unk_20 = vramRes;
    tmpl.x = ov64_021E6ECC[pos * 2];
    tmpl.y = ov64_021E6ECE[pos * 2];

    work->sprites[slot] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, (const ManagedSpriteTemplate *)&tmpl);
    loc = NNS_G2dGetImageLocation(Sprite_GetImageProxy(work->sprites[slot]->sprite), (NNS_G2D_VRAM_TYPE)(1));

    size = n * (n * 0x20);
    gender = GetGenderBySpeciesAndPersonality(work->mon.species, work->mon.personality);
    fileIdx = ov64_021E6E30(work->mon.species, work->mon.form, gender);
    res = AllocAndReadWholeNarcMemberByIdPair((NarcId)(0x51), fileIdx, (HeapID)(0x3b));
    tex = (u8 *)NNS_G3dGetTex(res);
    texImg = tex + *(int *)(tex + 0x14);
    buf = Heap_Alloc((HeapID)(0x3b), size);
    if ((u16)(work->mon.species + 0xFF9E) <= 1) {
        sub_020145B4(texImg + size * 6, n, 0, 0, n, n, buf);
        ov64_021E5AAC(buf, loc, size);
        sub_020145B4(texImg + size * 7, n, 0, 0, n, n, buf);
        ov64_021E5AAC(buf, loc + size, size);
    } else {
        sub_020145B4(texImg + size * 2, n, 0, 0, n, n, buf);
        ov64_021E5AAC(buf, loc, size);
        sub_020145B4(texImg + size * 3, n, 0, 0, n, n, buf);
        ov64_021E5AAC(buf, loc + size, size);
    }
    Heap_Free(buf);

    palLoc = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(work->sprites[slot]->sprite), (NNS_G2D_VRAM_TYPE)(1));
    palData = tex + *(int *)(tex + 0x38);
    if (CalcShininessByOtIdAndPersonality(work->mon.otid, work->mon.personality) != 0) {
        palData += 0x20;
    }
    ov64_021E5AE4(palData, palLoc, 0x20);
    Heap_Free(res);
}
