// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/mcss_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/pokemon.h"
#include "nnsys/g2d.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "system/mcss.h"

// The functions that fill in the files of a Pokémon's or a trainer's sprite for MCSS, and Spinda's spots. They follow
// mcss.c and are taken as a file of their own, whose name is a guess: the `.data` has mcss.c's name before this
// file's table of spots, where a file's literals come after its tables. SetupPokemonLoaderByBoxData,
// SetupPokemonLoaderFSTool and GetTrainerSpriteResIDs are swan's names

// A pixel of a spot in the sprite's 64x64 body, from the spot's place in the personality value
typedef struct {
    u8 x;
    u8 y;
} SpindaSpotPixel;

#define SPINDA_SPOT_COUNT 4
#define SPINDA_SPOT_END 0xff

// The bin file of Spinda's sprite in the Pokémon graphics archive
#define POKEGRA_SPINDA_BIN 6548

// The sprite's rows are 128 bytes, and its second frame is 40 rows below the first
#define SPINDA_ROW_SIZE 0x80
#define SPINDA_FRAME_ROWS 40

static void SetupPokemonLoaderByBoxData(BoxPkm *pkm, MCSSLoadInfo *info, u32 dir);
static BOOL func_020172bc(const MCSSLoadInfo *info, MCSSLoad *load, u32 personality);
static void func_020173c8(uintptr_t param, fx32 frame);

// pokebw2's sMcssSpindaSpot0
static const SpindaSpotPixel data_0209e17c[] = {
#include "asset/mcss_tool_data_0209e17c.inc"
};

// pokebw2's sMcssSpindaSpot1
static const SpindaSpotPixel data_0209e110[] = {
#include "asset/mcss_tool_data_0209e110.inc"
};

// pokebw2's sMcssSpindaSpot2
static const SpindaSpotPixel data_0209e0a8[] = {
#include "asset/mcss_tool_data_0209e0a8.inc"
};

// pokebw2's sMcssSpindaSpot3
static const SpindaSpotPixel data_0209e1e8[] = {
#include "asset/mcss_tool_data_0209e1e8.inc"
};

// pokebw2's sMcssSpindaSpots
static const SpindaSpotPixel *data_020a72d4[SPINDA_SPOT_COUNT] = {
    data_0209e17c,
    data_0209e110,
    data_0209e0a8,
    data_0209e1e8,
};

void func_02017100(PartyPkm *pkm, MCSSLoadInfo *info, u32 dir) {
    SetupPokemonLoaderByBoxData(func_0201d624(pkm), info, dir);
}

static void SetupPokemonLoaderByBoxData(BoxPkm *pkm, MCSSLoadInfo *info, u32 dir) {
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    BOOL rare = PML_PkmIsRare(pkm);

    SetupPokemonLoaderFSTool(species, form, sex, rare, PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL), info, dir);
}

void SetupPokemonLoaderFSTool(u32 species, u32 form, u32 sex, BOOL rare, BOOL egg, MCSSLoadInfo *info, u32 dir) {
    info->arcId = GetPokemonGraphicsARCID();
    info->character = GetPokemonCharacterDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->palette = GetPokemonPaletteDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->cells = GetPokemonCellsDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->cellAnime = GetPokemonCellAnimeDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->multiCells = GetPokemonMultiCellsDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->multiCellAnime = GetPokemonMultiCellAnimeDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->bin = GetPokemonBinFileDataNo(info->arcId, species, form, sex, rare, dir, egg);
    info->unk20 = 0;
}

void GetTrainerSpriteResIDs(u32 trainerType, MCSSLoadInfo *info, BOOL back) {
    info->arcId = GetTrainerSpriteARCID(back);
    info->character = GetTrSpriteCharacter2DatID(trainerType, back);
    info->palette = GetTrSpritePaletteDatID(trainerType, back);
    info->cells = GetTrSpriteCellDatID(trainerType, back);
    info->cellAnime = GetTrSpriteCellAnmDatID(trainerType, back);
    info->multiCells = GetTrSpriteMultiCellDatID(trainerType, back);
    info->multiCellAnime = GetTrSpriteMultiCellAnmDatID(trainerType, back);
    info->bin = GetTrSpriteBinDatID(trainerType, back);
    info->unk20 = 1;
}

MCSS *func_02017270(MCSSSystem *system, PartyPkm *pkm, u32 dir, fx32 x, fx32 y, fx32 z) {
    MCSSLoadInfo info;

    func_020172ac(system, PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL));
    func_02017100(pkm, &info, dir);
    return func_020159c8(system, x, y, z, &info);
}

void func_020172ac(MCSSSystem *system, u32 personality) {
    func_02014cf0(system, func_020172bc, personality);
}

static BOOL func_020172bc(const MCSSLoadInfo *info, MCSSLoad *load, u32 personality) {
    if (info->bin == POKEGRA_SPINDA_BIN) {
        func_020172d8(load->chars->pRawData, personality);
    }
    return TRUE;
}

// pokebw2's MCSS_DrawSpindaSpots
void func_020172d8(void *chars, u32 personality) {
    u8 *pixels = chars;
    const SpindaSpotPixel *spot;
    int i;
    int k;
    u8 j;

    for (i = 0; i < SPINDA_SPOT_COUNT; i++) {
        spot = data_020a72d4[i];
        j = 0;
        while (spot[j].x != SPINDA_SPOT_END) {
            u8 x = (personality & 0x0f) - 8 + spot[j].x;
            u8 y = ((personality & 0xf0) >> 4) - 8 + spot[j].y;
            int offsets[2];

            offsets[0] = x / 2 + y * SPINDA_ROW_SIZE;
            offsets[1] = x / 2 + (y + SPINDA_FRAME_ROWS) * SPINDA_ROW_SIZE;
            for (k = 0; k < 2; k++) {
                if (x & 1) {
                    if ((pixels[offsets[k]] & 0xf0) >= 0x10 && (pixels[offsets[k]] & 0xf0) <= 0x30) {
                        pixels[offsets[k]] += 0x50;
                    }
                } else {
                    if ((pixels[offsets[k]] & 0x0f) >= 0x01 && (pixels[offsets[k]] & 0x0f) <= 0x03) {
                        pixels[offsets[k]] += 0x05;
                    }
                }
            }
            j++;
        }
        personality >>= 8;
    }
}

// Restarts the sprite's animation each time it ends
void func_020173b4(MCSS *mcss) {
    func_02015f2c(mcss, (uintptr_t)mcss, func_020173c8, 1);
}

static void func_020173c8(uintptr_t param, fx32 frame) {
    func_02016380((MCSS *)param);
}
