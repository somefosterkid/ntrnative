// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pokegra.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/trainer_data.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "pml/personal.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"

// The files of Pokémon and trainer sprites, and loading them as cell actor resources. Named after the file name its
// allocations give. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except PokeGra_*, TrGra_*,
// func_0202da04, func_0202d6f4, the data and the constants

// Each species' files in ARCID_POKEGRA: the front's, then the back's, then the normal and the shiny palette
#define POKEGRA_FILE_SINGLE_CELL_CHARS 0
#define POKEGRA_FILE_CHARS 2
#define POKEGRA_FILE_CELLS 4
#define POKEGRA_FILE_CELL_ANIMS 5
#define POKEGRA_FILE_MULTI_CELLS 6
#define POKEGRA_FILE_MULTI_CELL_ANIMS 7
#define POKEGRA_FILE_BIN 8
#define POKEGRA_FILE_BACK 9
#define POKEGRA_FILE_PALETTE 18
#define POKEGRA_FILE_COUNT 20

// The sprites after the species': an egg's and a Manaphy egg's, then the forms' with sprites of their own, then the
// palettes (normal and shiny) of the forms that only change the palette
#define POKEGRA_EGG 650
#define POKEGRA_FORM_FILES 0x32f0
#define POKEGRA_FORM_PALETTES 0x37ad

// The size of a single-cell sprite's characters, 96 by 96 pixels
#define POKEGRA_CHARS_SIZE 0x1200

// Each trainer class's files in the trainer sprite archives
#define TRGRA_FILE_CHARS_1 0
#define TRGRA_FILE_CHARS_2 1
#define TRGRA_FILE_CELLS 2
#define TRGRA_FILE_CELL_ANIMS 3
#define TRGRA_FILE_MULTI_CELLS 4
#define TRGRA_FILE_MULTI_CELL_ANIMS 5
#define TRGRA_FILE_BIN 6
#define TRGRA_FILE_PALETTE 7
#define TRGRA_FILE_COUNT 8

typedef struct {
    u8 x;
    u8 y;
} SpindaSpotPixel;

static void func_0202d2ec(NNSG2dCharacterData *chars, HeapID heapId);
static void func_0202d3d0(NNSG2dCharacterData *chars, u32 personality);
static void GetPokemonDataIDBase(u32 arcId, int species, int form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 *base,
                                 u32 *dirOffset, u32 *sexOffset, u32 *rareOffset, u32 *formPalette, BOOL singleCell);
u32 GetTrSpriteCharacter1DatID(u32 trainerClass, u32 flags);
static void *LoadSingleCellSpindaGraphics(NNSG2dCharacterData **chars, u32 species, u32 form, u32 sex, BOOL rare,
                                          u32 dir, BOOL egg, u32 personality, HeapID heapId);

// The pixels of Spinda's four spots, relative to the spot's place in the sprite, which the personality moves
// pokebw2's sSpindaSpot1
static const SpindaSpotPixel data_020a0138[] = {
#include "asset/pokegra_data_020a0138.inc"
};

// pokebw2's sSpindaSpot2
static const SpindaSpotPixel data_020a00cc[] = {
#include "asset/pokegra_data_020a00cc.inc"
};

// pokebw2's sSpindaSpot3
static const SpindaSpotPixel data_020a0064[] = {
#include "asset/pokegra_data_020a0064.inc"
};

// pokebw2's sSpindaSpot4
static const SpindaSpotPixel data_020a01a4[] = {
#include "asset/pokegra_data_020a01a4.inc"
};

extern const u8 data_020a023c[106];

// pokebw2's sSpindaSpots
static const SpindaSpotPixel *data_020a779c[] = { data_020a0138, data_020a00cc, data_020a0064, data_020a01a4 };

u32 GetPokemonGraphicsARCID(void) {
    return ARCID_POKEGRA;
}

u32 GetPokemonSingleCellCharacterDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset, sexOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, &sexOffset, NULL, NULL, TRUE);
    return base + dirOffset + sexOffset;
}

u32 GetPokemonCharacterDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset, sexOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, &sexOffset, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_CHARS + sexOffset;
}

u32 GetPokemonPaletteDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, rareOffset;
    u32 formPalette = 0;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, NULL, NULL, &rareOffset, &formPalette,
                         FALSE);
    if (formPalette == 0) {
        formPalette = base + POKEGRA_FILE_PALETTE + rareOffset;
    }
    return formPalette;
}

u32 GetPokemonCellsDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, NULL, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_CELLS;
}

u32 GetPokemonCellAnimeDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, NULL, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_CELL_ANIMS;
}

u32 GetPokemonMultiCellsDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, NULL, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_MULTI_CELLS;
}

u32 GetPokemonMultiCellAnimeDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, NULL, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_MULTI_CELL_ANIMS;
}

u32 GetPokemonBinFileDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg) {
    u32 base, dirOffset;

    GetPokemonDataIDBase(arcId, species, form, sex, rare, dir, egg, &base, &dirOffset, NULL, NULL, NULL, FALSE);
    return base + dirOffset + POKEGRA_FILE_BIN;
}

// pokebw2's PokeGra_CellCharsToImage
void func_0202d214(NNSG2dCharacterData *chars, HeapID heapId) {
    u8 *raw = chars->pRawData;
    u8 *buf = GFL_HeapAllocate(HEAPID_TAIL(heapId), POKEGRA_CHARS_SIZE, TRUE, "pokegra.c", 256);
    u32 offset;
    int i;

    MIi_CpuCopyFast(raw, buf, POKEGRA_CHARS_SIZE);
    offset = 0;
    for (i = 0; i < 8; i++) {
        MIi_CpuCopyFast(buf + offset, raw + i * 0x180, 0x100);
        offset += 0x100;
    }
    for (i = 0; i < 8; i++) {
        MIi_CpuCopyFast(buf + offset, raw + (i * 12 + 8) * 32, 0x80);
        offset += 0x80;
    }
    for (i = 0; i < 4; i++) {
        MIi_CpuCopyFast(buf + offset, raw + (i * 0x180 + 0xc00), 0x100);
        offset += 0x100;
    }
    for (i = 0; i < 4; i++) {
        MIi_CpuCopyFast(buf + offset, raw + (i * 12 + 104) * 32, 0x80);
        offset += 0x80;
    }
    GFL_HeapFree(buf);
}

// Puts an image that func_0202d214 made back as the four OBJs' characters
// pokebw2's PokeGra_ImageToCellChars
static void func_0202d2ec(NNSG2dCharacterData *chars, HeapID heapId) {
    u8 *raw = chars->pRawData;
    u8 *buf = GFL_HeapAllocate(HEAPID_TAIL(heapId), POKEGRA_CHARS_SIZE, TRUE, "pokegra.c", 313);
    u32 offset;
    int i;

    MIi_CpuCopyFast(raw, buf, POKEGRA_CHARS_SIZE);
    offset = 0;
    for (i = 0; i < 8; i++) {
        MIi_CpuCopyFast(buf + i * 0x180, raw + offset, 0x100);
        offset += 0x100;
    }
    for (i = 0; i < 8; i++) {
        MIi_CpuCopyFast(buf + (i * 12 + 8) * 32, raw + offset, 0x80);
        offset += 0x80;
    }
    for (i = 0; i < 4; i++) {
        MIi_CpuCopyFast(buf + (i * 0x180 + 0xc00), raw + offset, 0x100);
        offset += 0x100;
    }
    for (i = 0; i < 4; i++) {
        MIi_CpuCopyFast(buf + (i * 12 + 104) * 32, raw + offset, 0x80);
        offset += 0x80;
    }
    GFL_HeapFree(buf);
}

// Draws Spinda's spots on an image that func_0202d214 made, each moved by a byte of the personality: its low
// nibble across and its high nibble down, from -8 to 7 pixels. Only the pixels of the face's colors (1 to 3) are
// spotted
// pokebw2's PokeGra_DrawSpindaSpots
static void func_0202d3d0(NNSG2dCharacterData *chars, u32 personality) {
    u8 *raw = chars->pRawData;
    int i, j;
    for (i = 0; i < 4; i++) {
        const SpindaSpotPixel *spot = data_020a779c[i];
        for (j = 0; spot[j].x != 0xff; j++) {
            int x = spot[j].x + 22 + ((personality & 0xf) - 8);
            int y = spot[j].y + 16 + (((personality & 0xf0) >> 4) - 8);
            int pos = ((x % 8) + (y % 8) * 8 + (x / 8 + (y / 8) * 12) * 64) / 2;

            if (x & 1) {
                if ((raw[pos] & 0xf0) >= 0x10 && (raw[pos] & 0xf0) <= 0x30) {
                    raw[pos] += 0x50;
                }
            } else {
                if ((raw[pos] & 0xf) >= 1 && (raw[pos] & 0xf) <= 3) {
                    raw[pos] += 5;
                }
            }
        }
        personality >>= 8;
    }
}

// The first file of the species' sprite, or of the form's, and the offsets of the direction's and the sex's files,
// which are the male's when the sex has none of its own. A form whose sprite is only a palette gives the palette's file
static void GetPokemonDataIDBase(u32 arcId, int species, int form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 *base,
                                 u32 *dirOffset, u32 *sexOffset, u32 *rareOffset, u32 *formPalette, BOOL singleCell) {
    u32 file = species * POKEGRA_FILE_COUNT;
    u32 dirFile = dir == POKEGRA_DIR_FRONT ? 0 : POKEGRA_FILE_BACK;
    u32 charsFile;

    if (singleCell) {
        charsFile = dir == POKEGRA_DIR_FRONT ? POKEGRA_FILE_SINGLE_CELL_CHARS : POKEGRA_FILE_BACK + POKEGRA_FILE_SINGLE_CELL_CHARS;
    } else {
        charsFile = dir == POKEGRA_DIR_FRONT ? POKEGRA_FILE_CHARS : POKEGRA_FILE_BACK + POKEGRA_FILE_CHARS;
    }

    if (egg) {
        file = (POKEGRA_EGG + (species == SPECIES_MANAPHY)) * POKEGRA_FILE_COUNT;
    } else if (form != 0) {
        u32 spriteOffset = PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_SPRITE_OFFSET);
        u32 paletteForms = PML_PersonalGetParamSingle(species, 0, PERSONAL_PALETTE_FORMS);
        int formCount = PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_COUNT);

        if (form >= formCount) {
            form = 0;
        }
        if (paletteForms != 0) {
            if (formPalette != NULL && form != 0) {
                *formPalette = POKEGRA_FORM_PALETTES + (spriteOffset + form - 1) * 2 + rare;
            }
        } else if (form != 0) {
            file = POKEGRA_FORM_FILES + (spriteOffset + form - 1) * POKEGRA_FILE_COUNT;
        }
    }

    switch (sex) {
    case GENDER_MALE:
        break;
    case GENDER_FEMALE:
        sex = GFL_ArcSysGetDataLength(arcId, file + charsFile + 1) != 0 ? GENDER_FEMALE : GENDER_MALE;
        break;
    case GENDER_UNKNOWN:
        sex = GENDER_MALE;
        break;
    }

    if (base != NULL) {
        *base = file;
    }
    if (dirOffset != NULL) {
        *dirOffset = dirFile;
    }
    if (sexOffset != NULL) {
        *sexOffset = sex;
    }
    if (rareOffset != NULL) {
        *rareOffset = rare;
    }
}

u32 GetTrainerSpriteARCID(BOOL back) {
    if (back) {
        return ARCID_TRSPRITE_BACK;
    }
    return ARCID_TRSPRITE_FRONT;
}

u32 GetTrSpriteCharacter1DatID(u32 trainerClass, u32 flags) {
    u32 base;
    if (flags & 1) {
        base = (u8)trainerClass;
    } else if (trainerClass > 0x69) {
        base = 0;
    } else {
        base = data_020a023c[trainerClass];
    }
    return base * TRGRA_FILE_COUNT;
}

u32 GetTrSpriteCharacter2DatID(u32 trainerClass, u32 flags) {
    u32 id;
    if (flags & 1) {
        id = (u8)trainerClass;
    } else {
        if (trainerClass > 0x69) {
            id = 0;
        } else {
            id = data_020a023c[trainerClass];
        }
    }
    return id * TRGRA_FILE_COUNT + TRGRA_FILE_CHARS_2;
}

u32 GetTrSpritePaletteDatID(u32 trainerClass, u32 flags) {
    u8 id;
    if (flags & 1) {
        id = trainerClass;
    } else if (trainerClass > 0x69) {
        id = 0;
    } else {
        id = data_020a023c[trainerClass];
    }
    return id * TRGRA_FILE_COUNT + TRGRA_FILE_PALETTE;
}

u32 GetTrSpriteCellDatID(u32 trainerClass, u32 flags) {
    u8 base;
    if (flags & 1) {
        base = trainerClass;
    } else if (trainerClass > 0x69) {
        base = 0;
    } else {
        base = data_020a023c[trainerClass];
    }
    return base * TRGRA_FILE_COUNT + TRGRA_FILE_CELLS;
}

u32 GetTrSpriteCellAnmDatID(u32 trainerClass, u32 flags) {
    u32 value;
    if (flags & 1) {
        value = (u8)trainerClass;
    } else {
        if (trainerClass > 0x69) {
            value = 0;
        } else {
            value = data_020a023c[trainerClass];
        }
    }
    return value * TRGRA_FILE_COUNT + TRGRA_FILE_CELL_ANIMS;
}

u32 GetTrSpriteMultiCellDatID(u32 trainerClass, u32 flags) {
    if (flags & 1) {
        trainerClass = (u8)trainerClass;
    } else {
        if (trainerClass > 0x69) {
            trainerClass = 0;
        } else {
            trainerClass = data_020a023c[trainerClass];
        }
    }
    return trainerClass * TRGRA_FILE_COUNT + TRGRA_FILE_MULTI_CELLS;
}

u32 GetTrSpriteMultiCellAnmDatID(u32 trainerClass, u32 flags) {
    u8 value;
    if (flags & 1) {
        value = trainerClass;
    } else {
        if (trainerClass > 0x69) {
            value = 0;
        } else {
            value = data_020a023c[trainerClass];
        }
    }
    return value * TRGRA_FILE_COUNT + TRGRA_FILE_MULTI_CELL_ANIMS;
}

u32 GetTrSpriteBinDatID(u32 trainerClass, u32 flags) {
    u8 id;
    if (flags & 1) {
        id = trainerClass;
    } else if (trainerClass > 0x69) {
        id = 0;
    } else {
        id = data_020a023c[trainerClass];
    }
    return id * TRGRA_FILE_COUNT + TRGRA_FILE_BIN;
}









// pokebw2's LoadSingleCellSpindaGraphicsByBoxData
void *func_0202d6f4(NNSG2dCharacterData **chars, BoxPkm *pkm, u32 dir, HeapID heapId) {
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    BOOL rare = PML_PkmIsRare(pkm);
    BOOL egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    u32 personality = PML_PkmGetParam(pkm, PKM_PARAM_PID, NULL);

    return LoadSingleCellSpindaGraphics(chars, species, form, sex, rare, dir, egg, personality, heapId);
}

// Loads a Pokémon's sprite as characters of one cell, and draws a Spinda's spots on them, returning the file to free
static void *LoadSingleCellSpindaGraphics(NNSG2dCharacterData **chars, u32 species, u32 form, u32 sex, BOOL rare,
                                          u32 dir, BOOL egg, u32 personality, HeapID heapId) {
    u32 fileId = GetPokemonSingleCellCharacterDataNo(GetPokemonGraphicsARCID(), species, form, sex, rare, dir, egg);
    void *file = GFL_G2DIOReadOBJNCGR(GetPokemonGraphicsARCID(), fileId, TRUE, chars, heapId);

    if (species == SPECIES_SPINDA) {
        func_0202d214(*chars, heapId);
        func_0202d3d0(*chars, personality);
        func_0202d2ec(*chars, heapId);
    }
    return file;
}

ArcTool *MakePokeGraArcHandle(HeapID heapId) {
    return GFL_ArcSysCreateFileHandle(GetPokemonGraphicsARCID(), heapId);
}

u32 func_0202d7d8(ArcTool *arc, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 vramType,
                             u16 offset, HeapID heapId) {
    return func_0204ac18(arc, GetPokemonPaletteDataNo(GetPokemonGraphicsARCID(), species, form, sex, rare, dir, egg),
                         vramType, offset, 0, 1, heapId);
}

u32 func_0202d81c(ArcTool *arc, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 personality,
                           u32 vramType, HeapID heapId) {
    u32 chars = func_0204a6c8(
        arc, GetPokemonSingleCellCharacterDataNo(GetPokemonGraphicsARCID(), species, form, sex, rare, dir, egg), TRUE,
        vramType, heapId);

    if (species == SPECIES_SPINDA) {
        NNSG2dCharacterData *charData;
        void *file = LoadSingleCellSpindaGraphics(&charData, species, form, sex, rare, dir, egg, personality, heapId);

        func_0204a9d0(chars, charData);
        GFL_HeapFree(file);
    }
    return chars;
}

u32 func_0202d898(u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 mapping, u32 vramType,
                               HeapID heapId) {
    u32 cellFile = func_02027710(mapping);
    u32 animFile = func_02027714(mapping);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    u32 cellAnims = func_0204af28(arc, cellFile, animFile, heapId);

    GFL_ArcToolFree(arc);
    return cellAnims;
}

// pokebw2's PokeGra_LoadClActPaletteByBoxData
u32 func_0202d8d0(ArcTool *arc, BoxPkm *pkm, u32 dir, u32 vramType, u16 offset, HeapID heapId) {
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    BOOL rare = PML_PkmIsRare(pkm);
    BOOL egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);

    return func_0202d7d8(arc, species, form, sex, rare, dir, egg, vramType, offset, heapId);
}

// pokebw2's PokeGra_LoadClActCharsByBoxData
u32 func_0202d934(ArcTool *arc, BoxPkm *pkm, u32 dir, u32 vramType, HeapID heapId) {
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    BOOL rare = PML_PkmIsRare(pkm);
    BOOL egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    u32 personality = PML_PkmGetParam(pkm, PKM_PARAM_PID, NULL);

    return func_0202d81c(arc, species, form, sex, rare, dir, egg, personality, vramType, heapId);
}

// pokebw2's PokeGra_LoadClActCellAnimsByBoxData
u32 func_0202d9a4(BoxPkm *pkm, u32 dir, u32 mapping, u32 vramType, HeapID heapId) {
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u32 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    u32 sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    BOOL rare = PML_PkmIsRare(pkm);
    BOOL egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);

    return func_0202d898(species, form, sex, rare, dir, egg, mapping, vramType, heapId);
}

// pokebw2's MakeTrGraArcHandle
ArcTool *func_0202da04(HeapID heapId) {
    return GFL_ArcSysCreateFileHandle(GetTrainerSpriteARCID(FALSE), heapId);
}

// pokebw2's TrGra_LoadClActPalette
u32 func_0202da18(ArcTool *arc, u32 trainerClass, u32 vramType, u16 offset, HeapID heapId) {
    return func_0204ac18(arc, GetTrSpritePaletteDatID(trainerClass, 0), vramType, offset, 0, 1, heapId);
}

// pokebw2's TrGra_LoadClActChars
u32 func_0202da48(ArcTool *arc, u32 trainerClass, u32 vramType, HeapID heapId) {
    return func_0204a6c8(arc, GetTrSpriteCharacter1DatID(trainerClass, 0), TRUE, vramType, heapId);
}

// pokebw2's TrGra_LoadClActCellAnims
u32 func_0202da6c(u32 trainerClass, u32 mapping, u32 vramType, HeapID heapId) {
    u32 cellFile = func_02027718(mapping);
    u32 animFile = func_0202771c(mapping);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    u32 cellAnims = func_0204af28(arc, cellFile, animFile, heapId);

    GFL_ArcToolFree(arc);
    return cellAnims;
}

// pokebw2's TrGra_ReplaceClActCharsAndPalette
void func_0202daa4(ArcTool *arc, u32 trainerClass, u32 chars, u32 palette, HeapID heapId) {
    void *file;
    NNSG2dPaletteData *paletteData;
    NNSG2dCharacterData *charData;

    file = GFL_ArcToolReadHeapNew(arc, GetTrSpritePaletteDatID(trainerClass, 0), heapId);
    NNS_G2dGetUnpackedPaletteData(file, &paletteData);
    func_0204adec(palette, paletteData, 1);
    GFL_HeapFree(file);

    file = GFL_G2DIOReadOBJNCGRArc(arc, GetTrSpriteCharacter1DatID(trainerClass, 0), TRUE, &charData, heapId);
    func_0204a9d0(chars, charData);
    GFL_HeapFree(file);
}
