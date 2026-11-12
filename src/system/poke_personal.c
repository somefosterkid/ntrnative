// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/poke_personal.c, ported by tools/port_file.py.
#include "types.h"
#include "pml/personal.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"

// The species' data of the personal archive. The file's name is the ROM's string; the functions' names are swan's,
// or dsd's where swan has none

// The file of ARCID_PERSONAL that holds the regional Pokédex numbers, and the number of a species that has none
#define PERSONAL_FILE_REGIONAL_DEX 668
#define REGIONAL_DEX_NONE 999

static u16 PML_PersonalGetDatID(u16 species, u16 form);

static ArcTool *g_PMLPersonalArcBW2;

void PML_PersonalArcInit(HeapID heapId) {
    if (g_PMLPersonalArcBW2 == NULL) {
        g_PMLPersonalArcBW2 = GFL_ArcSysCreateFileHandle(ARCID_PERSONAL, heapId);
    }
}

void PML_PersonalFetchBW2(u16 species, u16 form, PersonalData *dest) {
    u16 file = PML_PersonalGetDatID(species, form);

    GFL_ArcToolSeek(g_PMLPersonalArcBW2, GFL_ArcToolGetDataOfs(g_PMLPersonalArcBW2, file));
    GFL_ArcToolReadRaw(g_PMLPersonalArcBW2, sizeof(PersonalData), dest);
}

void PML_LearnsetLvUpLoad(u16 species, u16 form, void *dest) {
    GFL_ArcSysRead(dest, ARCID_LEARNSETS, PML_PersonalGetDatID(species, form));
}

void *PML_PersonalLoad(u16 species, u16 form, HeapID heapId) {
    PersonalData *personal = GFL_HeapAllocate(heapId, sizeof(PersonalData), FALSE, "poke_personal.c", 118);

    PML_PersonalFetchBW2(species, form, personal);
    return personal;
}

void PML_PersonalFree(void *personal) {
    GFL_HeapFree(personal);
}

u32 PML_PersonalGetParam(void *personal, u32 param) {
    PersonalData *data = personal;

    switch (param) {
    case 0:
        return data->hp;
    case 1:
        return data->attack;
    case 2:
        return data->defense;
    case 3:
        return data->speed;
    case 4:
        return data->spAttack;
    case 5:
        return data->spDefense;
    case PERSONAL_TYPE_1:
        return data->type1;
    case PERSONAL_TYPE_2:
        return data->type2;
    case 8:
        return data->catchRate;
    case 9:
        return data->baseExp;
    case 10:
        return data->evHP;
    case 11:
        return data->evAttack;
    case 12:
        return data->evDefense;
    case 13:
        return data->evSpeed;
    case 14:
        return data->evSpAttack;
    case 15:
        return data->evSpDefense;
    case PERSONAL_NO_BOUNCE:
        return data->noBounce;
    case 17:
        return data->items[0];
    case 18:
        return data->items[1];
    case 19:
        return data->items[2];
    case PERSONAL_SEX_RATIO:
        return data->sexRatio;
    case PERSONAL_HATCH_CYCLES:
        return data->hatchCycles;
    case 22:
        return data->friendship;
    case 23:
        return data->growthRate;
    case 24:
        return data->eggGroup1;
    case 25:
        return data->eggGroup2;
    case PERSONAL_ABILITY_1:
        return data->abilities[0];
    case PERSONAL_ABILITY_2:
        return data->abilities[1];
    case PERSONAL_ABILITY_HIDDEN:
        return data->abilities[2];
    case 29:
        return data->fleeRate;
    case 30:
        return data->formFile;
    case PERSONAL_FORM_SPRITE_OFFSET:
        return data->formSpriteOffset;
    case PERSONAL_FORM_COUNT:
        return data->formCount;
    case 33:
        return data->color;
    case PERSONAL_SPRITE_FLIP:
        return data->spriteFlip;
    case PERSONAL_PALETTE_FORMS:
        return data->paletteForms;
    case 36:
        return data->stage;
    case 37:
        return data->height;
    case PERSONAL_WEIGHT:
        return data->weight;
    case 39:
        return data->machines[0];
    case 40:
        return data->machines[1];
    case 41:
        return data->machines[2];
    case 42:
        return data->machines[3];
    case 43:
        return data->tutors;
    }
    return 0;
}

// Whether a sex ratio leaves one sex only: all male, all female, or none
BOOL isGenderlessOrSetGender(u32 genderRatio) {
    if (genderRatio == GENDER_RATIO_MALE_ONLY || genderRatio == GENDER_RATIO_FEMALE_ONLY ||
        genderRatio == GENDER_RATIO_GENDERLESS) {
        return TRUE;
    }
    return FALSE;
}

u16 *PML_PersonalLoadRegionalDexTable(HeapID heapId, u16 *count) {
    u32 size;
    u16 found = 0;
    u16 *table =
        GFL_ArcToolReadHeapNewLZGetLen(g_PMLPersonalArcBW2, PERSONAL_FILE_REGIONAL_DEX, FALSE, heapId, &size);
    u16 entries = size / sizeof(u16);

    if (count != NULL) {
        u16 i;

        for (i = 0; i < entries; i++) {
            if (table[i] != REGIONAL_DEX_NONE) {
                found++;
            }
        }
        *count = found;
    }
    return table;
}

// The file of a species' form in the personal and learnset archives: an egg's is 0, and a form's follows the species'
static u16 PML_PersonalGetDatID(u16 species, u16 form) {
    PersonalData personal;

    if (species == SPECIES_EGG || species == SPECIES_EGG + 1) {
        return 0;
    }
    GFL_ArcToolSeek(g_PMLPersonalArcBW2, GFL_ArcToolGetDataOfs(g_PMLPersonalArcBW2, species));
    GFL_ArcToolReadRaw(g_PMLPersonalArcBW2, sizeof(PersonalData), &personal);
    if (personal.formFile == 0 || personal.formCount <= form || form == 0) {
        return species;
    }
    return personal.formFile + form - 1;
}

