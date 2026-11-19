#include "global.h"
#include "constants/heap.h"
#include "constants/player_avatar.h"
#include "map_object.h"
#include "player_avatar.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "unk_02005D10.h"

struct LocalMapObject;

struct NARC;

struct PlayerAvatar;

struct PlayerData;

struct SysTask;

struct SysTaskManager;

enum AvatarBikeState {
    AVATAR_BIKE_STATE_NONE = 0,
    AVATAR_BIKE_STATE_MOVING,
    AVATAR_BIKE_STATE_TURNING,
    AVATAR_BIKE_STATE_BRAKE,
};

enum PlayerMovementActionSpeed {
    PLAYER_ACTION_SPEED_NONE,
    PLAYER_ACTION_SPEED_NOT_MOVING,
    PLAYER_ACTION_SPEED_SLOWER,
    PLAYER_ACTION_SPEED_SLOW,
    PLAYER_ACTION_SPEED_NORMAL,
    PLAYER_ACTION_SPEED_FAST,
    PLAYER_ACTION_SPEED_FASTER
};

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

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};

typedef struct PlayerData {
    u16 cyclingGear;
    u16 runningShoes;
    u32 playerState;
} PlayerData;

void Heap_Free(void *ptr);

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

LocalMapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);

BOOL PlayerAvatar_IsRunButtonHeld(PlayerAvatar *playerAvatar, u16 keyPress);

enum AvatarMoveState PlayerAvatar_GetMoveStateFromPlayerAvatarState(PlayerAvatar *playerAvatar, int dir);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}


enum MovementAction MovementAction_TurnActionTowardsDir(int targetDir, enum MovementAction movementAction);

enum FaceDirection PlayerAvatar_CalcFaceDirectionInternal(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys);

enum AvatarBikeState PlayerAvatar_UpdateBikeStateFourthGear(PlayerAvatar *playerAvatar, int dir);

enum AvatarBikeState sub_0205D7AC(PlayerAvatar *playerAvatar, int dir);

void PlayerAvatar_SetMovement(PlayerAvatar *playerAvatar, LocalMapObject *mapObj, enum MovementAction movementAction, int speed);

enum FaceDirection PlayerAvatar_CalcFaceDirection(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys);

void PlayerAvatar_SetMapObjMovement(PlayerAvatar *playerAvatar, enum MovementAction movementAction, int speed);

u32 sub_0205DF0C(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys, int actionSpeed, int isRunning, int _collision);

enum FaceDirection sub_0205DDD4(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys);

enum AvatarMoveState sub_0205DEC0(PlayerAvatar *playerAvatar, int dir);

enum MovementAction sub_0206234C(int targetDir, enum MovementAction movementAction);

BOOL sub_0205DE88(PlayerAvatar *playerAvatar, u32 keyPress);

u32 sub_0205DF0C(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys, int actionSpeed, int isRunning, int _collision) {
    u32 movementAction;
    int facingDir = sub_0205DDD4(playerAvatar, pressedKeys, heldKeys);
    int moveState = sub_0205DEC0(playerAvatar, facingDir);
    PlayerAvatar_SetMoveState(playerAvatar, moveState);
    if (moveState == AVATAR_MOVE_STATE_NONE) {
        facingDir = PlayerAvatar_GetFacingDirection(playerAvatar);
        movementAction = sub_0206234C(facingDir, MOVEMENT_ACTION_FACE_NORTH);
        return movementAction;
    }
    if (moveState == AVATAR_MOVE_STATE_TURNING) {
        movementAction = sub_0206234C(facingDir, MOVEMENT_ACTION_WALK_ON_SPOT_FASTER_NORTH);
        return movementAction;
    }
    PlayerAvatar_GetMapObject(playerAvatar);
    u32 collision = _collision;
    if (collision & (1 << 2)) {
        movementAction = MOVEMENT_ACTION_JUMP_FAR_NORTH;
    } else if (collision != 0) {
        movementAction = MOVEMENT_ACTION_WALK_ON_SPOT_SLOW_NORTH;
        if ((collision & (1 << 3)) == 0) {
            PlaySE(0x600);
        }
    } else {
        switch (actionSpeed) {
        case PLAYER_ACTION_SPEED_NONE:
            movementAction = MOVEMENT_ACTION_WALK_SLOWER_NORTH;
            break;

        case PLAYER_ACTION_SPEED_NOT_MOVING:
            movementAction = MOVEMENT_ACTION_WALK_SLOW_NORTH;
            break;

        case PLAYER_ACTION_SPEED_SLOWER:
            movementAction = MOVEMENT_ACTION_WALK_NORMAL_NORTH;
            break;

        case PLAYER_ACTION_SPEED_SLOW:
            movementAction = MOVEMENT_ACTION_WALK_SLIGHTLY_FAST_NORTH;
            break;

        case PLAYER_ACTION_SPEED_NORMAL:
            movementAction = MOVEMENT_ACTION_WALK_FAST_NORTH;
            break;

        case PLAYER_ACTION_SPEED_FAST:
            movementAction = MOVEMENT_ACTION_WALK_FASTER_NORTH;
            break;

        default:
            movementAction = MOVEMENT_ACTION_WALK_SLOWER_NORTH;
        }

        if ((isRunning == 1) && (sub_0205DE88(playerAvatar, heldKeys) == 1)) {
            movementAction = MOVEMENT_ACTION_RUN_NORTH;
        }
    }
    return sub_0206234C(facingDir, (enum MovementAction)movementAction);
}
