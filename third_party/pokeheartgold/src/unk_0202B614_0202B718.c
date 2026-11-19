#include "global.h"

#include "seal_case.h"
#include "unk_0202B614.h"
#include "pm_string.h"
#include "pokemon_types_def.h"

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
typedef u16 charcode_t;
void CopyStringToU16Array(const String *src, charcode_t *dst, u32 dstSize);
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
void CreateMon(Pokemon *mon, int monSpecies, int monLevel, int monIVs, BOOL useMonPersonalityParam, u32 monPersonality, int monOTIDSource, u32 monOTID);
void SetMonData(Pokemon *mon, int param, const void *value);
typedef struct PhotoAccessory {
    u8 unk_00;
    u8 xPos;
    u8 yPos;
    s8 priority;
} PhotoAccessory;
enum Accessory {
    ACCESSORY_WHITE_FLUFF = 0,
    ACCESSORY_YELLOW_FLUFF,
    ACCESSORY_PINK_FLUFF,
    ACCESSORY_BROWN_FLUFF,
    ACCESSORY_BLACK_FLUFF,
    ACCESSORY_ORANGE_FLUFF,
    ACCESSORY_ROUND_PEBBLE,
    ACCESSORY_GLITTER_BOULDER,
    ACCESSORY_SNAGGY_PEBBLE,
    ACCESSORY_JAGGED_BOULDER,
    ACCESSORY_BLACK_PEBBLE,
    ACCESSORY_MINI_PEBBLE,
    ACCESSORY_PINK_SCALE,
    ACCESSORY_BLUE_SCALE,
    ACCESSORY_GREEN_SCALE,
    ACCESSORY_PURPLE_SCALE,
    ACCESSORY_BIG_SCALE,
    ACCESSORY_NARROW_SCALE,
    ACCESSORY_BLUE_FEATHER,
    ACCESSORY_RED_FEATHER,
    ACCESSORY_YELLOW_FEATHER,
    ACCESSORY_WHITE_FEATHER,
    ACCESSORY_BLACK_MOUSTACHE,
    ACCESSORY_WHITE_MOUSTACHE,
    ACCESSORY_BLACK_BEARD,
    ACCESSORY_WHITE_BEARD,
    ACCESSORY_SMALL_LEAF,
    ACCESSORY_BIG_LEAF,
    ACCESSORY_NARROW_LEAF,
    ACCESSORY_SHED_CLAW,
    ACCESSORY_SHED_HOWN,
    ACCESSORY_THIN_MUSHROOM,
    ACCESSORY_THICK_MUSHROOM,
    ACCESSORY_STUMP,
    ACCESSORY_PRETTY_DEWDROP,
    ACCESSORY_SNOW_CRYSTAL,
    ACCESSORY_SPARKS,
    ACCESSORY_SHIMMERING_FIRE,
    ACCESSORY_MYSTIC_FIRE,
    ACCESSORY_DETERMINATION,
    ACCESSORY_PECULIAR_SPOON,
    ACCESSORY_PUFFY_SMOKE,
    ACCESSORY_POISON_EXTRACT,
    ACCESSORY_WEALTHY_COIN,
    ACCESSORY_EERIE_THING,
    ACCESSORY_SPRING,
    ACCESSORY_SEASHELL,
    ACCESSORY_HUMMING_NOTE,
    ACCESSORY_SHINY_POWDER,
    ACCESSORY_GLITTER_POWDER,
    ACCESSORY_RED_FLOWER,
    ACCESSORY_PINK_FLOWER,
    ACCESSORY_WHITE_FLOWER,
    ACCESSORY_BLUE_FLOWER,
    ACCESSORY_ORANGE_FLOWER,
    ACCESSORY_YELLOW_FLOWER,
    ACCESSORY_GOOGLY_SPECS,
    ACCESSORY_BLACK_SPECS,
    ACCESSORY_GORGEOUS_SPECS,
    ACCESSORY_SWEET_CANDY,
    ACCESSORY_CONFETTI,
    NON_UNIQUE_ACCESSORY_COUNT,
    ACCESSORY_COLORED_PARASOL = 61,
    ACCESSORY_OLD_UMBRELLA,
    ACCESSORY_SPOTLIGHT,
    ACCESSORY_CAPE,
    ACCESSORY_STANDING_MIKE,
    ACCESSORY_SURFBOARD,
    ACCESSORY_CARPET,
    ACCESSORY_RETRO_PIPE,
    ACCESSORY_FLUFFY_BED,
    ACCESSORY_MIRROR_BALL,
    ACCESSORY_PHOTO_BOARD,
    ACCESSORY_PINK_BARRETTE,
    ACCESSORY_RED_BARRETTE,
    ACCESSORY_BLUE_BARRETTE,
    ACCESSORY_YELLOW_BARRETTE,
    ACCESSORY_GREEN_BARRETTE,
    ACCESSORY_PINK_BALLOON,
    ACCESSORY_RED_BALLOONS,
    ACCESSORY_BLUE_BALLOONS,
    ACCESSORY_YELLOW_BALLOONS,
    ACCESSORY_GREEN_BALLOONS,
    ACCESSORY_LACE_HEADDRESS,
    ACCESSORY_TOP_HAT,
    ACCESSORY_SILK_VEIL,
    ACCESSORY_HEROIC_HEADBAND,
    ACCESSORY_PROFESSOR_HAT,
    ACCESSORY_FLOWER_STAGE,
    ACCESSORY_GOLD_PEDESTAL,
    ACCESSORY_GLASS_STAGE,
    ACCESSORY_AWARD_PODIUM,
    ACCESSORY_CUBE_STAGE,
    ACCESSORY_TURTWIG_MASK,
    ACCESSORY_CHIMCHAR_MASK,
    ACCESSORY_PIPLUP_MASK,
    ACCESSORY_BIG_TREE,
    ACCESSORY_FLAG,
    ACCESSORY_CROWN,
    ACCESSORY_TIARA,
    ACCESSORY_COMET,
    ACCESSORY_COUNT,
};

void sub_0202B718(PhotoPokemon *photoMon, const String *name, int gender);
void sub_0202B730(const PhotoPokemon *photoMon, Pokemon *mon);
void sub_0202B770(PhotoAccessory *accessory, u8 param1, u8 xPos, u8 yPos, u8 priority);

void sub_0202B718(PhotoPokemon *photoMon, const String *name, int gender) {
    CopyStringToU16Array(name, photoMon->trainerName, 7 + 1);
    photoMon->trainerGender = gender;
}

void sub_0202B730(const PhotoPokemon *photoMon, Pokemon *mon) {
    CreateMon(mon, photoMon->species, 0, 0, 1, photoMon->personality, 1, photoMon->otID);
    SetMonData(mon, MON_DATA_NICKNAME, photoMon->nickname);
    SetMonData(mon, MON_DATA_FORM, &photoMon->form);
}

void sub_0202B770(PhotoAccessory *accessory, u8 param1, u8 xPos, u8 yPos, u8 priority) {
    accessory->unk_00 = param1;
    accessory->xPos = xPos;
    accessory->yPos = yPos;
    accessory->priority = priority;
}

void sub_0202B784(u32 *flags, u8 count, u8 accessoryID) {
    ((accessoryID < NON_UNIQUE_ACCESSORY_COUNT) ? (void)0 : GF_AssertFail());

    u8 v0 = accessoryID / 8;
    u8 v1 = accessoryID % 8;

    v1 *= 4;

    flags[v0] &= ~(0xf << v1);
    flags[v0] |= (count << v1);
}

u8 sub_0202B7C8(const u32 *flags, u8 accessoryID) {
    u8 v0;
    u8 v1;
    u8 v2;

    ((accessoryID < NON_UNIQUE_ACCESSORY_COUNT) ? (void)0 : GF_AssertFail());

    v1 = accessoryID / 8;
    v2 = accessoryID % 8;
    v2 *= 4;
    v0 = (flags[v1] >> v2) & 0xf;

    if (v0 > 9) {
        v0 = 9;
    }

    return v0;
}
