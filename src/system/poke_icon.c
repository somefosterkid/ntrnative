// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/poke_icon.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "nitro/gx.h"
#include "pml/personal.h"
#include "pml/poke_party.h"

// The Pokémon icons of ARCID_POKEICON: the file and the palette of a Pokémon's icon, the files of the icons' palette,
// cells and animations, and a species packed with its form and sex. The ROM has no string for this file, so its name
// is a guess; the functions' names are swan's where they have one. The functions that pick the cells and animations
// by the OBJ mapping are kept here: they use no data that could show a second file, and they return files of the same
// archive

// The archive starts with the palette, a second palette that the PC boxes load, and the animations and cells for each OBJ
// mapping; then come two files for each icon, the second for the female where there is one
#define POKEICON_FILE_PALETTE 0
#define POKEICON_FILE_PALETTE_ALT 1
#define POKEICON_FILE_ANIM_32K 2
#define POKEICON_FILE_CELL_32K 3
#define POKEICON_FILE_ANIM_64K 4
#define POKEICON_FILE_CELL_64K 5
#define POKEICON_FILE_ANIM_128K 6
#define POKEICON_FILE_CELL_128K 7
#define POKEICON_FILE_ICONS 7

// The icons after the species': the egg, Manaphy's egg, and the forms, in the order of the form sprites
#define POKEICON_EGG 650
#define POKEICON_FORMS 652
// The same icons in the palette table, where the forms follow Manaphy's egg's entry
#define POKEICON_PALETTE_EGG 650
#define POKEICON_PALETTE_FORMS 651

// The species that keep six bits of form and no sex in a packed species
#define POKEICON_PACK_SPECIES_MASK 0x3ff
#define POKEICON_PACK_FORM_SHIFT 10
#define POKEICON_PACK_SEX_SHIFT 14

// The palettes of each icon: the male's, or the only one, in the low nibble and the female's in the high one
static const u8 data_0209e474[] = {
#include "asset/poke_icon_data_0209e474.inc"
};

// pokebw2's PokeIcon_GetFileOfPkm
u32 func_0201ba68(BoxPkm *pkm) {
    BOOL wasEncrypted;
    u32 species;
    u32 form;
    u32 sex;
    BOOL egg;
    u32 file;

    wasEncrypted = PML_PkmDecrypt(pkm);
    species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    file = PokeParty_GetIconIndex(species, form, sex, egg);
    PML_PkmReEncrypt(pkm, wasEncrypted);
    return file;
}

u32 PokeParty_GetIconIndex(u32 species, u32 form, u32 sex, BOOL egg) {
    u32 file = POKEICON_FILE_ICONS + species * 2;

    if (egg) {
        file = POKEICON_FILE_ICONS + ((species == SPECIES_MANAPHY) + POKEICON_EGG) * 2;
    } else if (form != 0) {
        u32 formOffset = PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_SPRITE_OFFSET);
        u32 paletteForms = PML_PersonalGetParamSingle(species, 0, PERSONAL_PALETTE_FORMS);

        if (form >= PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_COUNT)) {
            form = 0;
        }
        if (!paletteForms && form != 0) {
            file = (formOffset + form - 1) * 2 + (POKEICON_FILE_ICONS + POKEICON_FORMS * 2);
        }
    }

    // The female's icon, if the archive has one
    switch (sex) {
        case 0:
            break;
        case 1:
            sex = GFL_ArcSysGetDataLength(ARCID_POKEICON, file + 1) != 0 ? 1 : 0;
            break;
        case 2:
            sex = 0;
            break;
    }
    return file + sex;
}

// pokebw2's PokeIcon_GetPalette
u32 func_0201bb5c(u32 species, u32 form, u32 sex, BOOL egg) {
    if (egg) {
        species = (species == SPECIES_MANAPHY) + POKEICON_PALETTE_EGG;
    } else if (form != 0) {
        u32 formOffset = PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_SPRITE_OFFSET);
        u32 paletteForms = PML_PersonalGetParamSingle(species, 0, PERSONAL_PALETTE_FORMS);

        if (form >= PML_PersonalGetParamSingle(species, 0, PERSONAL_FORM_COUNT)) {
            form = 0;
        }
        if (!paletteForms && form != 0) {
            species = formOffset + form + POKEICON_PALETTE_FORMS;
        }
    }

    if (sex != 0) {
        return (u8)((data_0209e474[species] & 0xf0) >> 4);
    }
    return (u8)(data_0209e474[species] & 0xf);
}

// pokebw2's PokeIcon_GetPaletteOfPkm
u32 func_0201bbe8(BoxPkm *pkm) {
    BOOL wasEncrypted;
    u32 species;
    u32 form;
    u32 sex;
    BOOL egg;

    wasEncrypted = PML_PkmDecrypt(pkm);
    species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
    sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
    egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    PML_PkmReEncrypt(pkm, wasEncrypted);
    return func_0201bb5c(species, form, sex, egg);
}

u32 func_0201bc3c(void) {
    return 0;
}

u32 func_0201bc40(void) {
    switch (GX_GetOBJVRamModeChar()) {
        case GX_OBJVRAMMODE_CHAR_1D_32K:
            return POKEICON_FILE_ANIM_32K;
        case GX_OBJVRAMMODE_CHAR_1D_64K:
            return POKEICON_FILE_ANIM_64K;
        case GX_OBJVRAMMODE_CHAR_1D_128K:
            return POKEICON_FILE_ANIM_128K;
        default:
            return POKEICON_FILE_ANIM_128K;
    }
}

u32 func_0201bc78(void) {
    switch (GXS_GetOBJVRamModeChar()) {
        case GX_OBJVRAMMODE_CHAR_1D_32K:
            return POKEICON_FILE_ANIM_32K;
        case GX_OBJVRAMMODE_CHAR_1D_64K:
            return POKEICON_FILE_ANIM_64K;
        case GX_OBJVRAMMODE_CHAR_1D_128K:
            return POKEICON_FILE_ANIM_128K;
        default:
            return POKEICON_FILE_ANIM_128K;
    }
}

u32 getOBJTileMapping_MainEng(void) {
    switch (GX_GetOBJVRamModeChar()) {
        case GX_OBJVRAMMODE_CHAR_1D_32K:
            return 1;
        case GX_OBJVRAMMODE_CHAR_1D_64K:
            return 3;
        case GX_OBJVRAMMODE_CHAR_1D_128K:
            return 5;
        default:
            return 5;
    }
}

u32 getOBJTileMapping_SubEng(void) {
    switch (GXS_GetOBJVRamModeChar()) {
        case GX_OBJVRAMMODE_CHAR_1D_32K:
            return 1;
        case GX_OBJVRAMMODE_CHAR_1D_64K:
            return 3;
        case GX_OBJVRAMMODE_CHAR_1D_128K:
            return 5;
        default:
            return 5;
    }
}

