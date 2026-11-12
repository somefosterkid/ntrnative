// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/constants/battle.h: the lines in use so far.
#ifndef BWPORT_CONSTANTS_BATTLE_H
#define BWPORT_CONSTANTS_BATTLE_H

#include "types.h"

#define CONDITION_PARALYSIS 1
#define CONDITION_SLEEP 2
#define CONDITION_FREEZE 3
#define CONDITION_BURN 4
#define CONDITION_POISON 5

// Weather, from GetFieldWeather. The AI scripts tell which is which
#define BTL_WEATHER_SUN 1

#define BTL_WEATHER_RAIN 2
#define BTL_WEATHER_HAIL 3
#define BTL_WEATHER_SANDSTORM 4

#define CONDITION_CONFUSION 6

// Black 2's, not checked against Black
#define CONDITION_ATTRACT 7

#define CONDITION_BIND 8

// Black 2's, not checked against Black
#define CONDITION_NIGHTMARE 9
#define CONDITION_CURSE 10

#define CONDITION_TAUNT 11
#define CONDITION_TORMENT 12
#define CONDITION_DISABLE 13

// Black 2's, not checked against Black
#define CONDITION_YAWN 14

#define CONDITION_HEAL_BLOCK 15

// GuessAbility treats a Pokemon with this condition as having no ability
#define CONDITION_GASTRO_ACID 16

// Black 2's, not checked against Black
#define CONDITION_FORESIGHT 17
#define CONDITION_LEECH_SEED 18

#define CONDITION_EMBARGO 19
#define CONDITION_PERISH_SONG 20
#define CONDITION_INGRAIN 21
#define CONDITION_MEAN_LOOK 22
// The move Encore keeps the Pokemon to (func_ov095_0689beac ends it when the move has no PP left)
#define CONDITION_ENCORE 23

// Battle styles, from BtlSetup_GetBattleStyle
#define BTL_STYLE_SINGLE 0

#define BTL_STYLE_TRIPLE 2
#define BTL_STYLE_ROTATION 3

// Type effectiveness, which multiplies the power by 0, 1/4, 1/2, 1, 2 or 4
#define TYPE_EFFECTIVENESS_IMMUNE 0

// Black 2's, not checked against Black
#define TYPE_EFFECTIVENESS_HALF 2

#define TYPE_EFFECTIVENESS_NORMAL 3

// Black 2's, not checked against Black
#define TYPE_EFFECTIVENESS_DOUBLE 4

// Values for GetBattleMonStat. The stat stages go from 0 to 12, and are 6 when unchanged. The AI scripts tell which
// stage is which
#define BATTLEMON_ATTACK_STAGE 1

#define BATTLEMON_EVASION_STAGE 7
#define BATTLEMON_HP 13
#define FIELD_CONDITION_GRAVITY 2

#define MOVE_CATEGORY_PHYSICAL 1
#define MOVE_CATEGORY_SPECIAL 2

// Conditions of a battle Pokemon, which include the major status conditions. Named after the moves that inflict them
// in the move data, or from how the AI checks for them
#define CONDITION_NONE 0

#define BTL_STYLE_DOUBLE 1
#define BATTLEMON_LEVEL 15
#define BATTLEMON_FORM 19

#define BATTLEMON_ABILITY 16

// Black 2's, not checked against Black
#define TYPE_EFFECTIVENESS_QUADRUPLE 5

#define BATTLEMON_DEFENSE_STAGE 2
#define BATTLEMON_SP_ATTACK_STAGE 3
#define BATTLEMON_SP_DEFENSE_STAGE 4
#define BATTLEMON_SPEED_STAGE 5
#define BATTLEMON_ACCURACY_STAGE 6
#define BATTLEMON_ATTACK 8
#define BATTLEMON_SP_ATTACK 10
#define BATTLEMON_GENDER 18
#define BATTLEMON_STAT_STAGE_NEUTRAL 6

// Black 2's, not checked against Black
#define FIELD_CONDITION_WEATHER 0

// Black 2's, not checked against Black
#define FIELD_CONDITION_TRICK_ROOM 1
#define FIELD_CONDITION_IMPRISON 3
#define FIELD_CONDITION_WATER_SPORT 4
#define FIELD_CONDITION_MUD_SPORT 5
#define FIELD_CONDITION_WONDER_ROOM 6
#define FIELD_CONDITION_MAGIC_ROOM 7

// Black 2's, not checked against Black
#define TYPE_EFFECTIVENESS_QUARTER 1
#define SIDE_CONDITION_REFLECT 0

// Black 2's, not checked against Black
#define SIDE_CONDITION_LIGHT_SCREEN 1
#define SIDE_CONDITION_SAFEGUARD 2
#define SIDE_CONDITION_MIST 3
#define SIDE_CONDITION_TAILWIND 4
#define SIDE_CONDITION_LUCKY_CHANT 5
#define SIDE_CONDITION_SPIKES 6
#define SIDE_CONDITION_TOXIC_SPIKES 7
#define SIDE_CONDITION_STEALTH_ROCK 8
#define SIDE_CONDITION_WIDE_GUARD 9
#define SIDE_CONDITION_QUICK_GUARD 10
#define SIDE_CONDITION_RAINBOW 11
#define SIDE_CONDITION_SEA_OF_FIRE 12
#define SIDE_CONDITION_SWAMP 13
#define MOVE_TARGET_ALL_ADJACENT 4      // Every adjacent Pokémon but the user, as Surf and Earthquake
#define MOVE_TARGET_ADJACENT_FOES 5     // Every adjacent foe, as Growl and Rock Slide

#endif // BWPORT_CONSTANTS_BATTLE_H
