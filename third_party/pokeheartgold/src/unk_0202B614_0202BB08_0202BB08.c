#include "global.h"

#include "error_handling.h"
#include "pokemon_types_def.h"

typedef struct FashionCase {
    u32 nonUniqueAccessoryFlags[8];
    u32 uniqueAccessoryFlags[2];
    u32 backdropFlags[6];
} FashionCase;

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

void sub_0202B784(u32 *flags, u8 count, u8 accessoryID);

u8 sub_0202B7C8(const u32 *flags, u8 accessoryID);

void sub_0202B808(u32 *flags, u8 count, u8 accessoryID);

u8 sub_0202B848(const u32 *flags, u8 accessoryID);

BOOL sub_0202B8FC(u32 accessoryID);

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
BOOL sub_0202B614(u32 value);
BOOL sub_0202BC10(const DressUpPhoto *photo);
void sub_0202BC38(DressUpPhoto *photo);
void sub_0202BC60(DressUpPhoto *photo);

static inline u8 Accessory_ToUniqueID(u32 accessoryID) {
    ((accessoryID >= NON_UNIQUE_ACCESSORY_COUNT) ? (void)0 : GF_AssertFail());
    return accessoryID - NON_UNIQUE_ACCESSORY_COUNT;
}

void FashionCase_GiveFashionItem(FashionCase *fashionCase, u32 accessoryID, u32 amount);

void FashionCase_GiveFashionItem(FashionCase *fashionCase, u32 accessoryID, u32 amount) {
    u8 count;

    ((accessoryID < ACCESSORY_COUNT) ? (void)0 : GF_AssertFail());

    if (sub_0202B8FC(accessoryID)) {
        count = sub_0202B7C8(fashionCase->nonUniqueAccessoryFlags, accessoryID);
        count += amount;

        if (count > 9) {
            count = 9;
        }

        sub_0202B784(fashionCase->nonUniqueAccessoryFlags, count, accessoryID);
    } else {
        count = sub_0202B848(fashionCase->uniqueAccessoryFlags, accessoryID);
        count += amount;

        if (count > 1) {
            count = 1;
        }

        accessoryID = Accessory_ToUniqueID(accessoryID);
        sub_0202B808(fashionCase->uniqueAccessoryFlags, count, accessoryID);
    }
}

void sub_0202BB7C(FashionCase *fashionCase, u32 accessoryID, u32 amount);

void sub_0202BB7C(FashionCase *fashionCase, u32 accessoryID, u32 amount) {
    u8 count;

    GF_ASSERT(accessoryID < ACCESSORY_COUNT);

    if (sub_0202B8FC(accessoryID)) {
        count = sub_0202B7C8(fashionCase->nonUniqueAccessoryFlags, accessoryID);

        if (count > amount) {
            count -= amount;
        } else {
            count = 0;
        }

        sub_0202B784(fashionCase->nonUniqueAccessoryFlags, count, accessoryID);
    } else {
        count = 0;
        accessoryID = Accessory_ToUniqueID(accessoryID);

        sub_0202B808(fashionCase->uniqueAccessoryFlags, count, accessoryID);
    }
}

enum Backdrop {
    BACKDROP_DRESS_UP = 0,
    BACKDROP_RANCH = 1,
    BACKDROP_CITY_AT_NIGHT = 2,
    BACKDROP_SNOWY_TOWN = 3,
    BACKDROP_FIERY = 4,
    BACKDROP_OUTER_SPACE = 5,
    BACKDROP_CUMULUS_CLOUD = 6,
    BACKDROP_DESERT = 7,
    BACKDROP_FLOWER_PATCH = 8,
    BACKDROP_FUTURE_ROOM = 9,
    BACKDROP_OPEN_SEA = 10,
    BACKDROP_TOTAL_DARKNESS = 11,
    BACKDROP_TATAMI_ROOM = 12,
    BACKDROP_GINGERBREAD_ROOM = 13,
    BACKDROP_SEAFLOOR = 14,
    BACKDROP_UNDERGROUND = 15,
    BACKDROP_SKY = 16,
    BACKDROP_THEATER = 17,
    BACKDROP_COUNT = 18,
};

void sub_0202B870(u32 *flags, u8 count, u8 backdropID);

u8 sub_0202B8B4(const u32 *flags, u8 backdropID);

u8 sub_0202B8D8(const u32 *flags);

void FashionCase_GiveContestBackground(FashionCase *fashionCase, u32 backdropID);

void FashionCase_GiveContestBackground(FashionCase *fashionCase, u32 backdropID) {
    u8 count;

    GF_ASSERT(backdropID < BACKDROP_COUNT);

    if (sub_0202B8B4(fashionCase->backdropFlags, backdropID) == BACKDROP_COUNT) {
        count = sub_0202B8D8(fashionCase->backdropFlags);

        sub_0202B870(fashionCase->backdropFlags, count, backdropID);
    }
}

static inline BOOL DressUpPhoto_IsValid(const DressUpPhoto *photo) {
    return sub_0202B614(photo->integrity);
}

static inline void DressUpPhoto_InitInternal(DressUpPhoto *photo) {
    memset(photo, 0, sizeof(DressUpPhoto));
    photo->integrity = (0x1234);
}

BOOL sub_0202BC10(const DressUpPhoto *photo) {
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());

    if (photo->integrity == (0x2345)) {
        return 1;
    }

    return 0;
}

void sub_0202BC38(DressUpPhoto *photo) {
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());

    photo->integrity = (0x2345);
    photo->language = gGameLanguage;
}

void sub_0202BC60(DressUpPhoto *photo) {
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());
    DressUpPhoto_InitInternal(photo);
}
