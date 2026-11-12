// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/ribbon.c, which is the same code in Black
// 2/White 2's overlay 207. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "constants/pokemon.h"
#include "p_status_local.h"

// The ribbons as the summary screen shows them: each one's Pokémon field, icon and palette, name and description, and
// the category that numbers it. The name and description are lines of the summary screen's ribbon text, in the
// ribbons' order. The file's name is pokebw2's guess, after Gen 4's ribbon.c

typedef struct {
    u32 param;
    // Black's icons are Black 2's less 2; the rest of the table is the same
    u16 icon;
    u16 palette;
    u16 name;
    u16 description;
    u8 category;
} RibbonData;

// pokebw2's sRibbons
static const RibbonData data_ov131_021dc2f8[RIBBON_COUNT] = {
    // Champion Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 20, 24, 0, 0, 80, RIBBON_CATEGORY_LEAGUE },
    // Sinnoh Champ Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH, 25, 0, 1, 81, RIBBON_CATEGORY_LEAGUE },
    // Cool Ribbon
    { PKM_PARAM_RIBBON_G3_COOL, 16, 0, 2, 82, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Super
    { PKM_PARAM_RIBBON_G3_COOL + 1, 17, 0, 3, 83, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Hyper
    { PKM_PARAM_RIBBON_G3_COOL + 2, 18, 0, 4, 84, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Master
    { PKM_PARAM_RIBBON_G3_COOL + 3, 19, 0, 5, 85, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 4, 16, 1, 6, 86, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Super
    { PKM_PARAM_RIBBON_G3_COOL + 5, 17, 1, 7, 87, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Hyper
    { PKM_PARAM_RIBBON_G3_COOL + 6, 18, 1, 8, 88, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Master
    { PKM_PARAM_RIBBON_G3_COOL + 7, 19, 1, 9, 89, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 8, 16, 2, 10, 90, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Super
    { PKM_PARAM_RIBBON_G3_COOL + 9, 17, 2, 11, 91, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Hyper
    { PKM_PARAM_RIBBON_G3_COOL + 10, 18, 2, 12, 92, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Master
    { PKM_PARAM_RIBBON_G3_COOL + 11, 19, 2, 13, 93, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 12, 16, 3, 14, 94, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Super
    { PKM_PARAM_RIBBON_G3_COOL + 13, 17, 3, 15, 95, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Hyper
    { PKM_PARAM_RIBBON_G3_COOL + 14, 18, 3, 16, 96, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Master
    { PKM_PARAM_RIBBON_G3_COOL + 15, 19, 3, 17, 97, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 16, 16, 4, 18, 98, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Super
    { PKM_PARAM_RIBBON_G3_COOL + 17, 17, 4, 19, 99, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Hyper
    { PKM_PARAM_RIBBON_G3_COOL + 18, 18, 4, 20, 100, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Master
    { PKM_PARAM_RIBBON_G3_COOL + 19, 19, 4, 21, 101, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon
    { PKM_PARAM_RIBBON_G4_COOL, 20, 0, 22, 102, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Great
    { PKM_PARAM_RIBBON_G4_COOL + 1, 21, 0, 23, 103, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Ultra
    { PKM_PARAM_RIBBON_G4_COOL + 2, 22, 0, 24, 104, RIBBON_CATEGORY_CONTEST },
    // Cool Ribbon Master
    { PKM_PARAM_RIBBON_G4_COOL + 3, 23, 0, 25, 105, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon
    { PKM_PARAM_RIBBON_G4_COOL + 4, 20, 1, 26, 106, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Great
    { PKM_PARAM_RIBBON_G4_COOL + 5, 21, 1, 27, 107, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Ultra
    { PKM_PARAM_RIBBON_G4_COOL + 6, 22, 1, 28, 108, RIBBON_CATEGORY_CONTEST },
    // Beauty Ribbon Master
    { PKM_PARAM_RIBBON_G4_COOL + 7, 23, 1, 29, 109, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon
    { PKM_PARAM_RIBBON_G4_COOL + 8, 20, 2, 30, 110, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Great
    { PKM_PARAM_RIBBON_G4_COOL + 9, 21, 2, 31, 111, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Ultra
    { PKM_PARAM_RIBBON_G4_COOL + 10, 22, 2, 32, 112, RIBBON_CATEGORY_CONTEST },
    // Cute Ribbon Master
    { PKM_PARAM_RIBBON_G4_COOL + 11, 23, 2, 33, 113, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon
    { PKM_PARAM_RIBBON_G4_COOL + 12, 20, 3, 34, 114, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Great
    { PKM_PARAM_RIBBON_G4_COOL + 13, 21, 3, 35, 115, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Ultra
    { PKM_PARAM_RIBBON_G4_COOL + 14, 22, 3, 36, 116, RIBBON_CATEGORY_CONTEST },
    // Smart Ribbon Master
    { PKM_PARAM_RIBBON_G4_COOL + 15, 23, 3, 37, 117, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon
    { PKM_PARAM_RIBBON_G4_COOL + 16, 20, 4, 38, 118, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Great
    { PKM_PARAM_RIBBON_G4_COOL + 17, 21, 4, 39, 119, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Ultra
    { PKM_PARAM_RIBBON_G4_COOL + 18, 22, 4, 40, 120, RIBBON_CATEGORY_CONTEST },
    // Tough Ribbon Master
    { PKM_PARAM_RIBBON_G4_COOL + 19, 23, 4, 41, 121, RIBBON_CATEGORY_CONTEST },
    // Winning Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 21, 54, 0, 42, 122, RIBBON_CATEGORY_TOWER },
    // Victory Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 22, 55, 0, 43, 123, RIBBON_CATEGORY_TOWER },
    // Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 1, 56, 0, 44, 124, RIBBON_CATEGORY_TOWER },
    // Great Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 2, 57, 0, 45, 125, RIBBON_CATEGORY_TOWER },
    // Double Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 3, 58, 0, 46, 126, RIBBON_CATEGORY_TOWER },
    // Multi Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 4, 59, 0, 47, 127, RIBBON_CATEGORY_TOWER },
    // Pair Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 5, 60, 0, 48, 128, RIBBON_CATEGORY_TOWER },
    // World Ability Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 6, 61, 0, 49, 129, RIBBON_CATEGORY_TOWER },
    // Artist Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 23, 26, 1, 50, 130, RIBBON_CATEGORY_MEMORIAL },
    // Effort Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 24, 27, 2, 51, 131, RIBBON_CATEGORY_MEMORIAL },
    // Alert Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 7, 28, 2, 52, 132, RIBBON_CATEGORY_MEMORIAL },
    // Shock Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 8, 29, 0, 53, 133, RIBBON_CATEGORY_MEMORIAL },
    // Downcast Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 9, 30, 1, 54, 134, RIBBON_CATEGORY_MEMORIAL },
    // Careless Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 10, 31, 2, 55, 135, RIBBON_CATEGORY_MEMORIAL },
    // Relax Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 11, 32, 3, 56, 136, RIBBON_CATEGORY_MEMORIAL },
    // Snooze Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 12, 33, 0, 57, 137, RIBBON_CATEGORY_MEMORIAL },
    // Smile Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 13, 34, 2, 58, 138, RIBBON_CATEGORY_MEMORIAL },
    // Gorgeous Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 14, 35, 1, 59, 139, RIBBON_CATEGORY_MEMORIAL },
    // Royal Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 15, 36, 3, 60, 140, RIBBON_CATEGORY_MEMORIAL },
    // Gorgeous Royal Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 16, 37, 0, 61, 141, RIBBON_CATEGORY_MEMORIAL },
    // Footprint Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 17, 38, 0, 62, 142, RIBBON_CATEGORY_MEMORIAL },
    // Record Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 18, 39, 1, 63, 143, RIBBON_CATEGORY_MEMORIAL },
    // Legend Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 20, 40, 0, 64, 144, RIBBON_CATEGORY_MEMORIAL },
    // Country Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 28, 41, 3, 65, 145, RIBBON_CATEGORY_GIFT },
    // National Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 29, 41, 4, 66, 146, RIBBON_CATEGORY_GIFT },
    // Earth Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 30, 42, 0, 67, 147, RIBBON_CATEGORY_GIFT },
    // World Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 31, 42, 1, 68, 148, RIBBON_CATEGORY_GIFT },
    // Classic Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 26, 43, 1, 69, 149, RIBBON_CATEGORY_GIFT },
    // Premier Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 27, 44, 0, 70, 150, RIBBON_CATEGORY_GIFT },
    // Event Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 19, 45, 3, 71, 151, RIBBON_CATEGORY_GIFT },
    // Birthday Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 22, 46, 2, 72, 152, RIBBON_CATEGORY_GIFT },
    // Special Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 23, 47, 2, 73, 153, RIBBON_CATEGORY_GIFT },
    // Souvenir Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 24, 48, 1, 74, 154, RIBBON_CATEGORY_GIFT },
    // Wishing Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 25, 49, 0, 75, 155, RIBBON_CATEGORY_GIFT },
    // Battle Champion Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 25, 50, 3, 76, 156, RIBBON_CATEGORY_GIFT },
    // Regional Champion Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 26, 51, 3, 77, 157, RIBBON_CATEGORY_GIFT },
    // National Champion Ribbon
    { PKM_PARAM_RIBBON_G3_COOL + 27, 52, 1, 78, 158, RIBBON_CATEGORY_GIFT },
    // World Champion Ribbon
    { PKM_PARAM_RIBBON_CHAMPION_SINNOH + 21, 53, 1, 79, 159, RIBBON_CATEGORY_GIFT },
};

// pokebw2's Ribbon_GetData
u32 func_ov131_021dbf2c(u32 ribbon, u32 field) {
    switch (field) {
    case RIBBON_DATA_PARAM:
        return data_ov131_021dc2f8[ribbon].param;
    case RIBBON_DATA_ICON:
        return data_ov131_021dc2f8[ribbon].icon;
    case RIBBON_DATA_PALETTE:
        return data_ov131_021dc2f8[ribbon].palette;
    case RIBBON_DATA_NAME:
        return data_ov131_021dc2f8[ribbon].name;
    case RIBBON_DATA_DESCRIPTION:
        return data_ov131_021dc2f8[ribbon].description;
    case RIBBON_DATA_CATEGORY:
        return data_ov131_021dc2f8[ribbon].category;
    }
    return 0;
}

// pokebw2's Ribbon_GetDescription
u32 func_ov131_021dbf94(u32 ribbon) {
    return data_ov131_021dc2f8[ribbon].description;
}
