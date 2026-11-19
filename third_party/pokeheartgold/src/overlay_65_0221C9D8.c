#include "global.h"

#include "item.h"
#include "seal_case.h"
#include "pokemon_types_def.h"

enum SpeciesDataParam {
    SPECIES_DATA_BASE_HP = 0,
    SPECIES_DATA_BASE_ATK = 1,
    SPECIES_DATA_BASE_DEF = 2,
    SPECIES_DATA_BASE_SPEED = 3,
    SPECIES_DATA_BASE_SP_ATK = 4,
    SPECIES_DATA_BASE_SP_DEF = 5,
    SPECIES_DATA_TYPE_1 = 6,
    SPECIES_DATA_TYPE_2 = 7,
    SPECIES_DATA_CATCH_RATE = 8,
    SPECIES_DATA_BASE_EXP_REWARD = 9,
    SPECIES_DATA_EV_HP_YIELD = 10,
    SPECIES_DATA_EV_ATK_YIELD = 11,
    SPECIES_DATA_EV_DEF_YIELD = 12,
    SPECIES_DATA_EV_SPEED_YIELD = 13,
    SPECIES_DATA_EV_SP_ATK_YIELD = 14,
    SPECIES_DATA_EV_SP_DEF_YIELD = 15,
    SPECIES_DATA_HELD_ITEM_COMMON = 16,
    SPECIES_DATA_HELD_ITEM_RARE = 17,
    SPECIES_DATA_GENDER_RATIO = 18,
    SPECIES_DATA_HATCH_CYCLES = 19,
    SPECIES_DATA_BASE_FRIENDSHIP = 20,
    SPECIES_DATA_EXP_RATE = 21,
    SPECIES_DATA_EGG_GROUP_1 = 22,
    SPECIES_DATA_EGG_GROUP_2 = 23,
    SPECIES_DATA_ABILITY_1 = 24,
    SPECIES_DATA_ABILITY_2 = 25,
    SPECIES_DATA_SAFARI_FLEE_RATE = 26,
    SPECIES_DATA_BODY_COLOR = 27,
    SPECIES_DATA_FLIP_SPRITE = 28,
    SPECIES_DATA_TM_LEARNSET_MASK_1 = 29,
    SPECIES_DATA_TM_LEARNSET_MASK_2 = 30,
    SPECIES_DATA_TM_LEARNSET_MASK_3 = 31,
    SPECIES_DATA_TM_LEARNSET_MASK_4 = 32,
};
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
BOOL AcquireMonLock(Pokemon *mon);
BOOL ReleaseMonLock(Pokemon *mon, BOOL encrypt);
u32 GetMonData(Pokemon *mon, int param, void *dest);
u32 GetMonBaseStat_HandleAlternateForm(int monSpecies, int monForm, enum SpeciesDataParam param);
typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;

void ov65_0221C9D8(Pokemon *mon, TradeRoomMonDisplayData *displayData);

void ov65_0221C9D8(Pokemon *mon, TradeRoomMonDisplayData *displayData) {
    int wasDecrypted = AcquireMonLock(mon);

    displayData->pokeBall = GetMonData(mon, MON_DATA_POKEBALL, 0L);
    displayData->species = GetMonData(mon, MON_DATA_SPECIES, 0L);
    displayData->form = GetMonData(mon, MON_DATA_FORM, 0L);
    displayData->isEgg = GetMonData(mon, MON_DATA_IS_EGG, 0L);
    displayData->gender = GetMonData(mon, MON_DATA_GENDER, 0L);
    displayData->ballCapsuleId = GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0L);

    int item = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);

    ReleaseMonLock(mon, wasDecrypted);

    if (displayData->species != 0) {
        displayData->iconShouldFlip = GetMonBaseStat_HandleAlternateForm(displayData->species, displayData->form, SPECIES_DATA_FLIP_SPRITE);
    }

    if (item != 0) {
        displayData->heldItemType = ItemIdIsMail(item) + 1;
    }
}
