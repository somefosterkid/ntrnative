// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/constants/tr_ai.h: the lines in use so far.
#ifndef BWPORT_CONSTANTS_TR_AI_H
#define BWPORT_CONSTANTS_TR_AI_H

#include "types.h"

// The damage that the damage commands calculate: the lowest of the random rolls (85%), or a random roll. Gen 4 uses
// the highest roll where this game uses the lowest
#define USE_MIN_DAMAGE 0

// The Pokemon that a command refers to
#define AI_BATTLER_DEFENDER 0

#define AI_BATTLER_ATTACKER 1
#define AI_BATTLER_DEFENDER_PARTNER 2
#define AI_BATTLER_ATTACKER_PARTNER 3

// The type that LoadTypeFrom loads
#define LOAD_DEFENDER_TYPE_1 0

#define LOAD_ATTACKER_TYPE_1 1
#define LOAD_DEFENDER_TYPE_2 2
#define LOAD_ATTACKER_TYPE_2 3
#define LOAD_MOVE_TYPE 4
#define LOAD_DEFENDER_PARTNER_TYPE_1 5
#define LOAD_ATTACKER_PARTNER_TYPE_1 6
#define LOAD_DEFENDER_PARTNER_TYPE_2 7
#define LOAD_ATTACKER_PARTNER_TYPE_2 8

// How IfSpeedCompareEqualTo compares the attacker's speed with the defender's
#define COMPARE_SPEED_FASTER 0

#define COMPARE_SPEED_SLOWER 1
#define COMPARE_SPEED_TIE 2

#endif // BWPORT_CONSTANTS_TR_AI_H
