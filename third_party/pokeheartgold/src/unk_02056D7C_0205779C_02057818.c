#include "global.h"

#include "comm_player_manager.h"
#include "field_types_def.h"
#include "player_avatar.h"
#include "unk_02035900.h"

enum MovementAction {
    MOVEMENT_ACTION_FACE_NORTH = 0,
    MOVEMENT_ACTION_FACE_SOUTH = 1,
    MOVEMENT_ACTION_FACE_WEST = 2,
    MOVEMENT_ACTION_FACE_EAST = 3,
    MOVEMENT_ACTION_WALK_SLOWER_NORTH = 4,
    MOVEMENT_ACTION_WALK_SLOWER_SOUTH = 5,
    MOVEMENT_ACTION_WALK_SLOWER_WEST = 6,
    MOVEMENT_ACTION_WALK_SLOWER_EAST = 7,
    MOVEMENT_ACTION_WALK_SLOW_NORTH = 8,
    MOVEMENT_ACTION_WALK_SLOW_SOUTH = 9,
    MOVEMENT_ACTION_WALK_SLOW_WEST = 10,
    MOVEMENT_ACTION_WALK_SLOW_EAST = 11,
    MOVEMENT_ACTION_WALK_NORMAL_NORTH = 12,
    MOVEMENT_ACTION_WALK_NORMAL_SOUTH = 13,
    MOVEMENT_ACTION_WALK_NORMAL_WEST = 14,
    MOVEMENT_ACTION_WALK_NORMAL_EAST = 15,
    MOVEMENT_ACTION_WALK_FAST_NORTH = 16,
    MOVEMENT_ACTION_WALK_FAST_SOUTH = 17,
    MOVEMENT_ACTION_WALK_FAST_WEST = 18,
    MOVEMENT_ACTION_WALK_FAST_EAST = 19,
    MOVEMENT_ACTION_WALK_FASTER_NORTH = 20,
    MOVEMENT_ACTION_WALK_FASTER_SOUTH = 21,
    MOVEMENT_ACTION_WALK_FASTER_WEST = 22,
    MOVEMENT_ACTION_WALK_FASTER_EAST = 23,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOWER_NORTH = 24,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOWER_SOUTH = 25,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOWER_WEST = 26,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOWER_EAST = 27,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_NORTH = 28,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_SOUTH = 29,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_WEST = 30,
    MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_EAST = 31,
    MOVEMENT_ACTION_WALK_ON_SPOT_NORMAL_NORTH = 32,
    MOVEMENT_ACTION_WALK_ON_SPOT_NORMAL_SOUTH = 33,
    MOVEMENT_ACTION_WALK_ON_SPOT_NORMAL_WEST = 34,
    MOVEMENT_ACTION_WALK_ON_SPOT_NORMAL_EAST = 35,
    MOVEMENT_ACTION_WALK_ON_SPOT_FAST_NORTH = 36,
    MOVEMENT_ACTION_WALK_ON_SPOT_FAST_SOUTH = 37,
    MOVEMENT_ACTION_WALK_ON_SPOT_FAST_WEST = 38,
    MOVEMENT_ACTION_WALK_ON_SPOT_FAST_EAST = 39,
    MOVEMENT_ACTION_WALK_ON_SPOT_FASTER_NORTH = 40,
    MOVEMENT_ACTION_WALK_ON_SPOT_FASTER_SOUTH = 41,
    MOVEMENT_ACTION_WALK_ON_SPOT_FASTER_WEST = 42,
    MOVEMENT_ACTION_WALK_ON_SPOT_FASTER_EAST = 43,
    MOVEMENT_ACTION_JUMP_ON_SPOT_SLOW_NORTH = 44,
    MOVEMENT_ACTION_JUMP_ON_SPOT_SLOW_SOUTH = 45,
    MOVEMENT_ACTION_JUMP_ON_SPOT_SLOW_WEST = 46,
    MOVEMENT_ACTION_JUMP_ON_SPOT_SLOW_EAST = 47,
    MOVEMENT_ACTION_JUMP_ON_SPOT_FAST_NORTH = 48,
    MOVEMENT_ACTION_JUMP_ON_SPOT_FAST_SOUTH = 49,
    MOVEMENT_ACTION_JUMP_ON_SPOT_FAST_WEST = 50,
    MOVEMENT_ACTION_JUMP_ON_SPOT_FAST_EAST = 51,
    MOVEMENT_ACTION_JUMP_NEAR_FAST_NORTH = 52,
    MOVEMENT_ACTION_JUMP_NEAR_FAST_SOUTH = 53,
    MOVEMENT_ACTION_JUMP_NEAR_FAST_WEST = 54,
    MOVEMENT_ACTION_JUMP_NEAR_FAST_EAST = 55,
    MOVEMENT_ACTION_JUMP_FAR_NORTH = 56,
    MOVEMENT_ACTION_JUMP_FAR_SOUTH = 57,
    MOVEMENT_ACTION_JUMP_FAR_WEST = 58,
    MOVEMENT_ACTION_JUMP_FAR_EAST = 59,
    MOVEMENT_ACTION_DELAY_1 = 60,
    MOVEMENT_ACTION_DELAY_2 = 61,
    MOVEMENT_ACTION_DELAY_4 = 62,
    MOVEMENT_ACTION_DELAY_8 = 63,
    MOVEMENT_ACTION_DELAY_15 = 64,
    MOVEMENT_ACTION_DELAY_16 = 65,
    MOVEMENT_ACTION_DELAY_32 = 66,
    MOVEMENT_ACTION_WARP_OUT = 67,
    MOVEMENT_ACTION_WARP_IN = 68,
    MOVEMENT_ACTION_SET_INVISIBLE = 69,
    MOVEMENT_ACTION_SET_VISIBLE = 70,
    MOVEMENT_ACTION_LOCK_DIR = 71,
    MOVEMENT_ACTION_UNLOCK_DIR = 72,
    MOVEMENT_ACTION_PAUSE_ANIMATION = 73,
    MOVEMENT_ACTION_RESUME_ANIMATION = 74,
    MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK = 75,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FAST_NORTH = 76,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FAST_SOUTH = 77,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FAST_WEST = 78,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FAST_EAST = 79,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FASTER_NORTH = 80,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FASTER_SOUTH = 81,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FASTER_WEST = 82,
    MOVEMENT_ACTION_WALK_SLIGHTLY_FASTER_EAST = 83,
    MOVEMENT_ACTION_WALK_FASTEST_NORTH = 84,
    MOVEMENT_ACTION_WALK_FASTEST_SOUTH = 85,
    MOVEMENT_ACTION_WALK_FASTEST_WEST = 86,
    MOVEMENT_ACTION_WALK_FASTEST_EAST = 87,
    MOVEMENT_ACTION_RUN_NORTH = 88,
    MOVEMENT_ACTION_RUN_SOUTH = 89,
    MOVEMENT_ACTION_RUN_WEST = 90,
    MOVEMENT_ACTION_RUN_EAST = 91,
    MOVEMENT_ACTION_JUMP_NEAR_SLOW_WEST = 92,
    MOVEMENT_ACTION_JUMP_NEAR_SLOW_EAST = 93,
    MOVEMENT_ACTION_JUMP_FARTHER_WEST = 94,
    MOVEMENT_ACTION_JUMP_FARTHER_EAST = 95,
    MOVEMENT_ACTION_WALK_EVER_SO_SLIGHTLY_FAST_NORTH = 96,
    MOVEMENT_ACTION_WALK_EVER_SO_SLIGHTLY_FAST_SOUTH = 97,
    MOVEMENT_ACTION_WALK_EVER_SO_SLIGHTLY_FAST_WEST = 98,
    MOVEMENT_ACTION_WALK_EVER_SO_SLIGHTLY_FAST_EAST = 99,
    MOVEMENT_ACTION_POKECENTER_NURSE_BOW = 100,
    MOVEMENT_ACTION_REVEAL_TRAINER = 101,
    MOVEMENT_ACTION_PLAYER_GIVE = 102,
    MOVEMENT_ACTION_EMOTE_DOUBLE_EXCLAMATION_MARK = 103,
    MOVEMENT_ACTION_PLAYER_RECEIVE = 104,
    MOVEMENT_ACTION_105 = 105,
    MOVEMENT_ACTION_106 = 106,
    MOVEMENT_ACTION_107 = 107,
    MOVEMENT_ACTION_108 = 108,
    MOVEMENT_ACTION_109 = 109,
    MOVEMENT_ACTION_110 = 110,
    MOVEMENT_ACTION_111 = 111,
    MOVEMENT_ACTION_112 = 112,
    MOVEMENT_ACTION_113 = 113,
    MOVEMENT_ACTION_114 = 114,
    MOVEMENT_ACTION_115 = 115,
    MOVEMENT_ACTION_116 = 116,
    MOVEMENT_ACTION_JUMP_DISTORTION_WORLD_NORTH = 117,
    MOVEMENT_ACTION_JUMP_DISTORTION_WORLD_SOUTH = 118,
    MOVEMENT_ACTION_JUMP_DISTORTION_WORLD_WEST = 119,
    MOVEMENT_ACTION_JUMP_DISTORTION_WORLD_EAST = 120,
    MOVEMENT_ACTION_121 = 121,
    MOVEMENT_ACTION_122 = 122,
    MOVEMENT_ACTION_123 = 123,
    MOVEMENT_ACTION_124 = 124,
    MOVEMENT_ACTION_125 = 125,
    MOVEMENT_ACTION_126 = 126,
    MOVEMENT_ACTION_127 = 127,
    MOVEMENT_ACTION_128 = 128,
    MOVEMENT_ACTION_129 = 129,
    MOVEMENT_ACTION_130 = 130,
    MOVEMENT_ACTION_131 = 131,
    MOVEMENT_ACTION_132 = 132,
    MOVEMENT_ACTION_133 = 133,
    MOVEMENT_ACTION_134 = 134,
    MOVEMENT_ACTION_135 = 135,
    MOVEMENT_ACTION_136 = 136,
    MOVEMENT_ACTION_137 = 137,
    MOVEMENT_ACTION_138 = 138,
    MOVEMENT_ACTION_139 = 139,
    MOVEMENT_ACTION_140 = 140,
    MOVEMENT_ACTION_141 = 141,
    MOVEMENT_ACTION_142 = 142,
    MOVEMENT_ACTION_143 = 143,
    MOVEMENT_ACTION_144 = 144,
    MOVEMENT_ACTION_145 = 145,
    MOVEMENT_ACTION_146 = 146,
    MOVEMENT_ACTION_147 = 147,
    MOVEMENT_ACTION_148 = 148,
    MOVEMENT_ACTION_149 = 149,
    MOVEMENT_ACTION_150 = 150,
    MOVEMENT_ACTION_151 = 151,
    MOVEMENT_ACTION_152 = 152,
    MOVEMENT_ACTION_153 = 153,
    MAX_MOVEMENT_ACTION = 154,
    MOVEMENT_ACTION_END = 254,
    MOVEMENT_ACTION_NONE = 255,
};

int sub_02057524(int param0);

u32 sub_0205DF0C(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys, int actionSpeed, int isRunning, int _collision);

enum MovementAction sub_0206234C(int targetDir, enum MovementAction movementAction);

s32 sub_0205DE38(PlayerAvatar *);

s32 sub_0205DFC8(PlayerAvatar *);

s32 sub_0205DFD4(PlayerAvatar *, u32);


void sub_02057818(s32 netId);

void sub_02057818(s32 netId) {
    PlayerAvatar *sp18;
    s32 sp14;
    s32 sp10;
    u32 spC;
    s32 sp8;
    PlayerAvatar *temp_r0;
    enum MovementAction var_r7;
    s32 temp_r0_2;
    s32 temp_r1_2;
    s32 temp_r4;
    s32 temp_r7;
    s8 temp_r0_4;
    s8 temp_r0_5;
    u16 var_r4;
    u8 temp_r0_3;
    u8 temp_r0_6;
    u8 temp_r1;
    u8 *temp_r1_3;
    u8 *temp_r2;
    CommPlayerPosition *temp_r6;

    temp_r6 = &_021D41C4->positions[netId];
    temp_r2 = _021D41C4->moveTimer;
    temp_r1 = temp_r2[netId];
    if (temp_r1 != 0) {
        temp_r2[netId] = temp_r1 - 1;
        return;
    }
    if (temp_r6->x != 0xFFFF) {
        if (temp_r6->z != 0xFFFF) {
            if (_021D41C4->unk_EE == 0) {
                if (netId == sub_0203769C()) {
                    if (_021D41C4->localWalkEnabled == 0) {

                    } else {
                        goto block_8;
                    }
                } else {
block_8:
                    temp_r0 = sp18 = _021D41C4->avatars[netId];
                    if (temp_r0 == NULL) {
                        return;
                    }
                    temp_r0_2 = sp10 = PlayerAvatar_GetXCoord(temp_r0) - temp_r6->x;
                    temp_r7 = PlayerAvatar_GetZCoord(sp18) - temp_r6->z;
                    spC = PlayerAvatar_GetFacingDirection(sp18);
                    if (temp_r0_2 == 0) {
                        if (temp_r7 == 0) {
                            var_r4 = 0;
                        } else {
                            goto block_13;
                        }
                    } else {
block_13:
                        temp_r4 = abs(sp10);
                        if (temp_r4 > abs(temp_r7)) {
                            if (sp10 > 0) {
                                var_r4 = 0x20;
                            } else {
                                var_r4 = 0x10;
                            }
                        } else if (temp_r7 > 0) {
                            var_r4 = 0x40;
                        } else {
                            var_r4 = 0x80;
                        }
                    }
                    abs(temp_r7);
                    abs(sp10);
                    temp_r0_3 = temp_r6->speed;
                    var_r7 = MOVEMENT_ACTION_NONE;
                    sp14 = temp_r0_3;
                    switch (temp_r0_3) {
                    case 0:
                        sp8 = 5;
                        break;
                    case 1:
                        sp8 = 4;
                        var_r4 |= 2;
                        break;
                    case 2:
                        sp8 = 2;
                        break;
                    case 3:
                        sp8 = 1;
                        break;
                    }
                    if (_021D41C4->localMoveDelay != 0) {
                        if (netId == sub_0203769C()) {
                            _021D41C4->localMoveDelay = (u8)(_021D41C4->localMoveDelay - 1);
                        } else {
                            goto block_29;
                        }
                    } else {
block_29:
                        temp_r1_2 = var_r4 & ~2;
                        if (temp_r1_2 == 0) {
                            if (temp_r6->bumped != 0) {
                                sp14 = 3;
                                temp_r0_4 = temp_r6->facing;
                                switch (temp_r0_4) {
                                case 0:
                                    var_r7 = MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_NORTH;
                                    break;
                                default:
                                case 1:
                                    var_r7 = MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_SOUTH;
                                    break;
                                case 2:
                                    var_r7 = MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_WEST;
                                    break;
                                case 3:
                                    var_r7 = MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_EAST;
                                    break;
                                }
                            } else {
                                goto block_37;
                            }
                        } else {
block_37:
                            if (temp_r1_2 == 0) {
                                temp_r0_5 = temp_r6->facing;
                                if (temp_r0_5 != spC) {
                                    var_r7 = sub_0206234C((s32)temp_r0_5, MOVEMENT_ACTION_WALK_ON_SPOT_FAST_NORTH);
                                } else {
                                    goto block_40;
                                }
                            } else {
block_40:
                                var_r7 = (enum MovementAction)sub_0205DF0C(sp18, var_r4, var_r4, sp8, 1, 0);
                            }
                        }
                    }
                    if (sub_0205DFC8(sp18) == 0) {
                        if (sub_0205DE38(sp18) != 0) {
                            goto block_43;
                        }
                    } else {
block_43:
                        if (var_r7 != MOVEMENT_ACTION_NONE) {
                            sub_0205DFD4(sp18, (u32)var_r7);
                            if (var_r4 & ~2) {
                                if (_021D41C4->moveTimer[netId] == 0) {
                                    _021D41C4->moveTimer[netId] = (s8)sub_02057524(sp14);
                                }
                                temp_r1_3 = _021D41C4->moveTimer;
                                temp_r0_6 = temp_r1_3[netId];
                                if (temp_r0_6 != 0) {
                                    temp_r1_3[netId] = temp_r0_6 - 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
