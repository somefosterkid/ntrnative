#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

BOOL ov07_0221BFC0(BattleAnimSystem *system);

enum Battler {
    BTLSCR_ALL_BATTLERS = 0,
    BTLSCR_ATTACKER = 1,
    BTLSCR_DEFENDER = 2,
    BTLSCR_PLAYER = 3,
    BTLSCR_ENEMY = 4,
    BTLSCR_FAINTED_MON = 5,
    BTLSCR_SWITCHED_MON = 6,
    BTLSCR_SIDE_EFFECT_MON = 7,
    BTLSCR_ABILITY_MON = 8,
    BTLSCR_PLAYER_SLOT_1 = 9,
    BTLSCR_ENEMY_SLOT_1 = 10,
    BTLSCR_PLAYER_SLOT_2 = 11,
    BTLSCR_ENEMY_SLOT_2 = 12,
    BTLSCR_UNUSED_13 = 13,
    BTLSCR_MSG_ATTACKER = 14,
    BTLSCR_MSG_DEFENDER = 15,
    BTLSCR_ATTACKER_PARTNER = 16,
    BTLSCR_DEFENDER_PARTNER = 17,
    BTLSCR_FORCED_OUT = 18,
    BTLSCR_ATTACKER_ENEMY = 19,
    BTLSCR_DEFENDER_ENEMY = 20,
    BTLSCR_MSG_BATTLER_TEMP = 21,
    BTLSCR_SWITCHED_MON_AFTER = 22,
};

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

s8 ov07_0221F8C8(BattleAnimSystem *system, s8 pan);

s8 ov07_0221F8C8(BattleAnimSystem *system, s8 pan) {
    int adjustedPan = pan;
    int attacker = ov07_0221C468(system);
    int defender = ov07_0221C470(system);
    (void)(ov07_0223192C(system, attacker));
    (void)(ov07_0223192C(system, defender));

    if ((ov07_0223192C(system, attacker) == BTLSCR_PLAYER) && (ov07_0223192C(system, defender) == BTLSCR_ENEMY)) {

    } else if ((ov07_0223192C(system, attacker) == BTLSCR_ENEMY) && (ov07_0223192C(system, defender) == BTLSCR_PLAYER)) {
        adjustedPan *= -1;
    } else if ((ov07_0223192C(system, attacker) == BTLSCR_PLAYER) && (ov07_0223192C(system, defender) == BTLSCR_PLAYER)) {
        if (adjustedPan > 0) {
            adjustedPan *= -1;
        }
    } else if ((ov07_0223192C(system, attacker) == BTLSCR_ENEMY) && (ov07_0223192C(system, defender) == BTLSCR_ENEMY)) {
        if (adjustedPan < 0) {
            adjustedPan *= -1;
        }
    }

    if (ov07_0221BFC0(system) == 1) {
        adjustedPan *= -1;
    }

    return adjustedPan;
}
