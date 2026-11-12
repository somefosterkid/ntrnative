// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_tables.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_tables.h"
#include "types.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"

static BOOL data_ov095_0689c889(u16 value, const u16 *list, u32 count);
static BOOL func_ov095_0689ca0c(u16 move);

// Searches a list of ids
// pokebw2's U16List_Contains
static BOOL data_ov095_0689c889(u16 value, const u16 *list, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        if (value == list[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's sEncoreExcludedMoves
static const u16 data_ov095_0689e2da[] = { MOVE_NONE,      MOVE_ENCORE, MOVE_MIRROR_MOVE,
                                            MOVE_TRANSFORM, MOVE_MIMIC,  MOVE_SKETCH };

// pokebw2's IsEncoreExcludedMove
BOOL data_ov095_0689c8ad(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e2da, 6);
}

// pokebw2's sMeFirstExcludedMoves
static const u16 data_ov095_0689e31c[] = { MOVE_NONE,        MOVE_MIRROR_COAT, MOVE_COUNTER, MOVE_METAL_BURST,
                                             MOVE_FOCUS_PUNCH, MOVE_ME_FIRST,    MOVE_THIEF,   MOVE_COVET,
                                             MOVE_CHATTER,     MOVE_STRUGGLE };

// pokebw2's IsMeFirstExcludedMove
BOOL data_ov095_0689c8bd(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e31c, 10);
}

// pokebw2's sDelayedAttackMoves
static const u16 data_ov095_0689e2b6[] = { MOVE_NONE, MOVE_FUTURE_SIGHT, MOVE_DOOM_DESIRE };

// pokebw2's IsDelayedAttackMove
BOOL data_ov095_0689c8cd(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e2b6, 3);
}

// pokebw2's sPressureFieldMoves
static const u16 data_ov095_0689e2c4[] = { MOVE_SNATCH, MOVE_IMPRISON, MOVE_SPIKES, MOVE_TOXIC_SPIKES,
                                           MOVE_STEALTH_ROCK };

// pokebw2's IsPressureFieldMove
BOOL data_ov095_0689c8dd(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e2c4, 5);
}

// pokebw2's sPledgeMoves
static const u16 data_ov095_0689e2b0[] = { MOVE_FIRE_PLEDGE, MOVE_GRASS_PLEDGE, MOVE_WATER_PLEDGE };

// pokebw2's IsPledgeMove
BOOL data_ov095_0689c8ed(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e2b0, 3);
}

// pokebw2's sReactiveHeldItems
static const u16 data_ov095_0689e2f6[] = { ITEM_FOCUS_BAND,   ITEM_POWER_HERB,  ITEM_EJECT_BUTTON,
                                          ITEM_JABOCA_BERRY, ITEM_ROWAP_BERRY, ITEM_CUSTAP_BERRY,
                                          ITEM_QUICK_CLAW,   ITEM_ABSORB_BULB, ITEM_CELL_BATTERY };

// pokebw2's IsReactiveHeldItem
BOOL data_ov095_0689c8fd(u16 item) {
    return data_ov095_0689c889(item, data_ov095_0689e2f6, 9);
}

// pokebw2's sEscapeItems
static const u16 data_ov095_0689e2aa[] = { ITEM_FLUFFY_TAIL, ITEM_POKE_DOLL, ITEM_POKE_TOY };

// pokebw2's IsEscapeItem
BOOL data_ov095_0689c90d(u16 item) {
    return data_ov095_0689c889(item, data_ov095_0689e2aa, 3);
}

// pokebw2's sRolePlayExcludedAbilities
static const u16 data_ov095_0689e2e6[] = { ABILITY_TRACE,       ABILITY_FORECAST,    ABILITY_MULTITYPE,
                                                  ABILITY_FLOWER_GIFT, ABILITY_ZEN_MODE,    ABILITY_ILLUSION,
                                                  ABILITY_IMPOSTER,    ABILITY_WONDER_GUARD };

// pokebw2's IsRolePlayExcludedAbility
BOOL data_ov095_0689c91d(u16 ability) {
    if (IsAbilityMultitype(ability)) {
        return TRUE;
    }
    return data_ov095_0689c889(ability, data_ov095_0689e2e6, 8);
}

// pokebw2's IsEntrainmentExcludedAbility
BOOL data_ov095_0689c93d(u16 ability) {
    return data_ov095_0689c945(ability);
}

// pokebw2's IsTraceExcludedAbility
BOOL data_ov095_0689c945(u16 ability) {
    if (ability == ABILITY_WONDER_GUARD) {
        return FALSE;
    }
    return data_ov095_0689c91d(ability);
}

// pokebw2's sSkillSwapExcludedAbilities
static const u16 data_ov095_0689e2a4[] = { ABILITY_WONDER_GUARD, ABILITY_MULTITYPE, ABILITY_ILLUSION };

// pokebw2's IsSkillSwapExcludedAbility
BOOL data_ov095_0689c955(u16 ability) {
    if (IsAbilityMultitype(ability)) {
        return TRUE;
    }
    return data_ov095_0689c889(ability, data_ov095_0689e2a4, 3);
}

BOOL IsAbilityMultitype(u16 ability) {
    return ability == ABILITY_MULTITYPE;
}

// pokebw2's sArceusPlates
static const u16 data_ov095_0689e38c[] = { ITEM_FLAME_PLATE,  ITEM_SPLASH_PLATE, ITEM_ZAP_PLATE,    ITEM_MEADOW_PLATE,
                                     ITEM_ICICLE_PLATE, ITEM_FIST_PLATE,   ITEM_TOXIC_PLATE,  ITEM_EARTH_PLATE,
                                     ITEM_SKY_PLATE,    ITEM_MIND_PLATE,   ITEM_INSECT_PLATE, ITEM_STONE_PLATE,
                                     ITEM_SPOOKY_PLATE, ITEM_DRACO_PLATE,  ITEM_DREAD_PLATE,  ITEM_IRON_PLATE };

// pokebw2's IsArceusPlate
BOOL data_ov095_0689c981(u16 item) {
    return data_ov095_0689c889(item, data_ov095_0689e38c, 16);
}

// pokebw2's sGenesectDrives
static const u16 data_ov095_0689e2bc[] = { ITEM_DOUSE_DRIVE, ITEM_SHOCK_DRIVE, ITEM_BURN_DRIVE, ITEM_CHILL_DRIVE };

// pokebw2's IsGenesectDrive
BOOL data_ov095_0689c991(u16 item) {
    return data_ov095_0689c889(item, data_ov095_0689e2bc, 4);
}

// pokebw2's sBackgrounds
static const u16 data_ov095_0689e308[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 };

// pokebw2's IsDuskBallBackground
BOOL data_ov095_0689c9a1(u16 background) {
    return data_ov095_0689c889(background, data_ov095_0689e308, 10);
}

// The abilities that Mold Breaker and its kin ignore
// pokebw2's sMoldBreakerIgnoredAbilities
static const u16 data_ov095_0689e450[] = {
    ABILITY_WONDER_GUARD, ABILITY_SOUNDPROOF,   ABILITY_LEVITATE,     ABILITY_SAND_VEIL,    ABILITY_SNOW_CLOAK,
    ABILITY_WATER_ABSORB, ABILITY_BATTLE_ARMOR, ABILITY_LIGHTNINGROD, ABILITY_STORM_DRAIN,  ABILITY_SHELL_ARMOR,
    ABILITY_UNAWARE,      ABILITY_LIGHTNINGROD, ABILITY_STORM_DRAIN,  ABILITY_SUCTION_CUPS, ABILITY_SIMPLE,
    ABILITY_TANGLED_FEET, ABILITY_SOLID_ROCK,   ABILITY_FILTER,       ABILITY_FLASH_FIRE,   ABILITY_MOTOR_DRIVE,
    ABILITY_STICKY_HOLD,  ABILITY_MARVEL_SCALE, ABILITY_THICK_FAT,    ABILITY_HEATPROOF,    ABILITY_WHITE_SMOKE,
    ABILITY_CLEAR_BODY,   ABILITY_KEEN_EYE,     ABILITY_HYPER_CUTTER, ABILITY_INNER_FOCUS,  ABILITY_SHIELD_DUST,
    ABILITY_STURDY,       ABILITY_DAMP,         ABILITY_OBLIVIOUS,    ABILITY_LIMBER,       ABILITY_OWN_TEMPO,
    ABILITY_WATER_VEIL,   ABILITY_LEAF_GUARD,   ABILITY_INSOMNIA,     ABILITY_BIG_PECKS,    ABILITY_VITAL_SPIRIT,
    ABILITY_IMMUNITY,     ABILITY_MAGMA_ARMOR,  ABILITY_CONTRARY,     ABILITY_FRIEND_GUARD, ABILITY_MULTISCALE,
    ABILITY_TELEPATHY,    ABILITY_BIG_PECKS,    ABILITY_WONDER_SKIN,  ABILITY_MAGIC_BOUNCE, ABILITY_SAP_SIPPER,
    ABILITY_VOLT_ABSORB,  ABILITY_DRY_SKIN,     ABILITY_FLOWER_GIFT,  ABILITY_HEAVY_METAL,  ABILITY_LIGHT_METAL
};

// pokebw2's IsMoldBreakerIgnoredAbility
BOOL data_ov095_0689c9b1(u16 ability) {
    return data_ov095_0689c889(ability, data_ov095_0689e450, 55);
}

BOOL IsArceus(u16 species) {
    return species == SPECIES_ARCEUS;
}

// Ids that the stat-effect command takes; they are neither moves nor items, so they stay numbers
// pokebw2's sEffectIds
static const u16 data_ov095_0689e35c[] = { 598, 602, 599, 600, 601, 603, 604, 627, 628, 642, 606, 611 };

// pokebw2's IsStatEffectId
BOOL data_ov095_0689c9d5(u16 effect) {
    return data_ov095_0689c889(effect, data_ov095_0689e35c, 12);
}

// pokebw2's sConditions
static const u32 data_ov095_0689e374[] = { CONDITION_ATTRACT, CONDITION_TORMENT, CONDITION_DISABLE, CONDITION_HEAL_BLOCK, 23,
                                   CONDITION_TAUNT };

// pokebw2's GetMentalHerbCondition
u32 data_ov095_0689c9e5(u32 index) {
    if (index < 6) {
        return data_ov095_0689e374[index];
    }
    return 0;
}

// pokebw2's sConditionsAll
static const u32 data_ov095_0689e4c0[] = { 35,
                                      CONDITION_INGRAIN,
                                      CONDITION_LEECH_SEED,
                                      CONDITION_POISON,
                                      CONDITION_BURN,
                                      CONDITION_NIGHTMARE,
                                      CONDITION_CURSE,
                                      CONDITION_BIND,
                                      CONDITION_TAUNT,
                                      CONDITION_TORMENT,
                                      23,
                                      CONDITION_DISABLE,
                                      30,
                                      32,
                                      CONDITION_HEAL_BLOCK,
                                      CONDITION_EMBARGO,
                                      28,
                                      29,
                                      CONDITION_YAWN,
                                      CONDITION_PERISH_SONG,
                                      CONDITION_PARALYSIS,
                                      CONDITION_SLEEP,
                                      CONDITION_FREEZE,
                                      CONDITION_ATTRACT,
                                      CONDITION_CONFUSION,
                                      CONDITION_GASTRO_ACID,
                                      CONDITION_FORESIGHT,
                                      CONDITION_MEAN_LOOK,
                                      24,
                                      25,
                                      26,
                                      27,
                                      31,
                                      33,
                                      34 };

// pokebw2's GetConditionByIndex
u32 data_ov095_0689c9f9(u32 index) {
    if (index < 35) {
        return data_ov095_0689e4c0[index];
    }
    return 0;
}

// pokebw2's sUncallableMoves
static const u16 data_ov095_0689e330[] = { MOVE_METRONOME, MOVE_SLEEP_TALK,  MOVE_ASSIST,       MOVE_COPYCAT,
                                        MOVE_ME_FIRST,  MOVE_MIRROR_MOVE, MOVE_NATURE_POWER, MOVE_CHATTER,
                                        MOVE_STRUGGLE,  MOVE_SKETCH,      MOVE_MIMIC };

// The moves that call other moves, and the Pokemon's own
// pokebw2's IsUncallableMove
static BOOL func_ov095_0689ca0c(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e330, 11);
}

// pokebw2's sSleepTalkExcludedMoves
static const u16 data_ov095_0689e346[] = { MOVE_UPROAR,       MOVE_SKULL_BASH,  MOVE_SKY_ATTACK, MOVE_SOLAR_BEAM,
                                               MOVE_BIDE,         MOVE_RAZOR_WIND,  MOVE_FLY,        MOVE_BOUNCE,
                                               MOVE_SHADOW_FORCE, MOVE_FOCUS_PUNCH, MOVE_SKY_DROP };

// pokebw2's IsSleepTalkExcludedMove
BOOL data_ov095_0689ca1d(u16 move) {
    if (func_ov095_0689ca0c(move)) {
        return TRUE;
    }
    return data_ov095_0689c889(move, data_ov095_0689e346, 11);
}

// pokebw2's sAssistExcludedMoves
static const u16 data_ov095_0689e3d4[] = { MOVE_COUNTER,   MOVE_MIRROR_COAT,  MOVE_PROTECT,     MOVE_DETECT,
                                            MOVE_ENDURE,    MOVE_DESTINY_BOND, MOVE_FOLLOW_ME,   MOVE_RAGE_POWDER,
                                            MOVE_SNATCH,    MOVE_HELPING_HAND, MOVE_THIEF,       MOVE_COVET,
                                            MOVE_TRICK,     MOVE_SWITCHEROO,   MOVE_FEINT,       MOVE_FOCUS_PUNCH,
                                            MOVE_TRANSFORM, MOVE_BESTOW,       MOVE_DRAGON_TAIL, MOVE_CIRCLE_THROW };

// pokebw2's IsAssistExcludedMove
BOOL data_ov095_0689ca3d(u16 move) {
    if (func_ov095_0689ca0c(move)) {
        return TRUE;
    }
    return data_ov095_0689c889(move, data_ov095_0689e3d4, 20);
}

// pokebw2's sCopycatExcludedMoves
static const u16 data_ov095_0689e3ac[] = { MOVE_COUNTER,   MOVE_MIRROR_COAT,  MOVE_PROTECT,     MOVE_DETECT,
                                             MOVE_ENDURE,    MOVE_DESTINY_BOND, MOVE_FOLLOW_ME,   MOVE_RAGE_POWDER,
                                             MOVE_SNATCH,    MOVE_HELPING_HAND, MOVE_THIEF,       MOVE_COVET,
                                             MOVE_TRICK,     MOVE_SWITCHEROO,   MOVE_FEINT,       MOVE_FOCUS_PUNCH,
                                             MOVE_TRANSFORM, MOVE_BESTOW,       MOVE_DRAGON_TAIL, MOVE_CIRCLE_THROW };

// pokebw2's IsCopycatExcludedMove
BOOL data_ov095_0689ca5d(u16 move) {
    if (func_ov095_0689ca0c(move)) {
        return TRUE;
    }
    return data_ov095_0689c889(move, data_ov095_0689e3ac, 20);
}

// pokebw2's sMimicExcludedMoves
static const u16 data_ov095_0689e2ce[] = { MOVE_NONE,      MOVE_SKETCH,   MOVE_MIMIC,
                                           MOVE_TRANSFORM, MOVE_STRUGGLE, MOVE_CHATTER };

// pokebw2's IsMimicExcludedMove
BOOL data_ov095_0689ca7d(u16 move) {
    return data_ov095_0689c889(move, data_ov095_0689e2ce, 6);
}

// pokebw2's sMetronomeExcludedMoves
static const u16 data_ov095_0689e3fc[] = {
    MOVE_METRONOME,    MOVE_SLEEP_TALK,   MOVE_STRUGGLE,     MOVE_ASSIST,       MOVE_COPYCAT,      MOVE_ME_FIRST,
    MOVE_MIRROR_MOVE,  MOVE_NATURE_POWER, MOVE_CHATTER,      MOVE_SKETCH,       MOVE_MIMIC,        MOVE_COUNTER,
    MOVE_MIRROR_COAT,  MOVE_PROTECT,      MOVE_DETECT,       MOVE_ENDURE,       MOVE_DESTINY_BOND, MOVE_TRANSFORM,
    MOVE_FOLLOW_ME,    MOVE_RAGE_POWDER,  MOVE_FOCUS_PUNCH,  MOVE_SNATCH,       MOVE_HELPING_HAND, MOVE_THIEF,
    MOVE_COVET,        MOVE_TRICK,        MOVE_SWITCHEROO,   MOVE_FEINT,        MOVE_QUASH,        MOVE_BESTOW,
    MOVE_TECHNO_BLAST, MOVE_RELIC_SONG,   MOVE_SECRET_SWORD, MOVE_FREEZE_SHOCK, MOVE_ICE_BURN,     MOVE_SNARL,
    MOVE_V_CREATE,     MOVE_SNORE,        MOVE_QUICK_GUARD,  MOVE_WIDE_GUARD,   MOVE_AFTER_YOU
};

// pokebw2's GetMetronomeExcludedMoves
const u16 *data_ov095_0689ca8d(u32 *count) {
    *count = 41;
    return data_ov095_0689e3fc;
}
