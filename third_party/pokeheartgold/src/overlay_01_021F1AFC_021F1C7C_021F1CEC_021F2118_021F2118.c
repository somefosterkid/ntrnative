#include "global.h"

#include "field/field_control.h"

#include "field_bgm.h"
#include "field_system.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_01_021FE780.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_vars_flags.h"
#include "sound_chatot.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_0205FD20.h"
#include "unk_02062108.h"

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

enum MovementAction sub_0206234C(int targetDir, enum MovementAction movementAction);

void ov01_021F3054(FieldSystem *, MonRideTask *);

BOOL ov01_021F3068(MonRideTask *);

void ov01_021F3084(PlayerAvatar *, u32);

void ov01_021F30F4(SurfTaskEnv *);

void ov01_02205EE0(TaskManager *);

BOOL ov01_021F2118(TaskManager *param_1);

BOOL ov01_021F2118(TaskManager *param_1) {
    SurfTaskEnv *env;
    SaveVarsFlags *state;
    u32 uVar2;
    s32 iVar3;
    s32 iVar4;
    enum MovementAction a1;
    BOOL BVar5;
    u32 uVar6;
    LocalMapObject *object;
    u8 a1_00;

    env = TaskManager_GetEnvironment(param_1);
    switch (env->state) {
    case 0:
        state = Save_VarsFlags_Get(env->fieldSystem->saveData);
        BVar5 = CheckFlag99A(state);
        if ((BVar5 == 0) && (iVar4 = SndRadio_GetSeqNo(), iVar4 == 0)) {
            FieldBGM_SetOverride(env->fieldSystem, 0);
            FieldBGM_TryFadeOut(env->fieldSystem, 0x3f6, 1);
        }
        env->state = env->state + 1;
        env->state = env->state + 1;
        break;
    case 1:
        iVar4 = env->timer;
        env->timer = iVar4 + 1;
        if (10 < iVar4 + 1) {
            env->timer = 0;
            env->state = env->state + 1;
        }
        break;
    case 2:
        if (env->monRideTask.playCutIn == 1) {
            ov01_021F3054(env->fieldSystem, &env->monRideTask);
            env->state = 5;
        } else {
            BVar5 = ov02_02250780(env->fieldSystem, 0xb);
            if (BVar5 != 0) {
                FieldSystem_UnkSub108_AddMonMood(env->fieldSystem->unk108, 1);
                a1_00 = 2;
            } else {
                a1_00 = 1;
            }
            ov02_022507B4(env->fieldSystem, a1_00);
            env->state = 3;
        }
        break;
    case 3:
        uVar6 = GetMonData(env->monRideTask.partyMon, 5, (void *)0x0);
        uVar2 = GetMonData(env->monRideTask.partyMon, 0x70, (void *)0x0);
        PlayCry((u16)uVar6, (u8)uVar2);
        env->state = 4;
        break;
    case 4:
        BVar5 = IsCryFinished();
        if (BVar5 == 0) {
            ov01_02205EE0(param_1);
            env->state = 6;
        }
        break;
    case 5:
        iVar4 = ov01_021F3068(&env->monRideTask);
        if (iVar4 == 1) {
            ov01_02205D68(env->fieldSystem);
            env->state = 6;
        }
        break;
    case 6:
        uVar6 = PlayerAvatar_GetXCoord(env->playerAvatar);
        iVar4 = GetDeltaXByFacingDirection(env->direction);
        uVar2 = PlayerAvatar_GetZCoord(env->playerAvatar);
        iVar3 = GetDeltaYByFacingDirection(env->direction);
        env->animManager = ov01_021FE7DC(env->playerObject, uVar6 + iVar4, uVar2 + iVar3, env->direction, 0);
        PlayerAvatar_SetUnk34(env->playerAvatar, env->animManager);
        PlayerAvatar_SetState(env->playerAvatar, 2);
        env->state = env->state + 1;
        break;
    case 7:
        iVar4 = env->timer;
        env->timer = iVar4 + 1;
        if (10 < iVar4 + 1) {
            uVar6 = PlayerAvatar_GetGender(env->playerAvatar);
            uVar6 = PlayerAvatar_GetSpriteByStateAndGender(0, uVar6);
            ov01_021F3084(env->playerAvatar, uVar6);
            env->timer = 0;
            env->state = env->state + 1;
        }
        break;
    case 8:
        BVar5 = MapObject_AreBitsSetForMovementScriptInit(env->playerObject);
        if (BVar5 == 1) {
            a1 = sub_0206234C(env->direction, MOVEMENT_ACTION_JUMP_NEAR_FAST_NORTH);
            MapObject_SetHeldMovement(env->playerObject, a1);
            env->state = env->state + 1;
        }
        break;
    case 9:
        BVar5 = MapObject_IsMovementPaused(env->playerObject);
        if (BVar5 != 0) {
            MapObject_ClearHeldMovementIfActive(env->playerObject);
            ov01_021FE9F4(env->animManager, 1);
            uVar6 = PlayerAvatar_GetGender(env->playerAvatar);
            uVar6 = PlayerAvatar_GetSpriteByStateAndGender(2, uVar6);
            ov01_021F3084(env->playerAvatar, uVar6);
            BVar5 = FollowMon_IsActive(env->fieldSystem);
            if (BVar5 != 0) {
                ov01_02205790(env->fieldSystem, (u8)env->direction);
                object = FollowMon_GetMapObject(env->fieldSystem);
                sub_0205FC94(object, 0x38);
            }
            FieldSystem_ProcessSoundplate(env->fieldSystem, 0);
            ov01_021F30F4(env);
            return 1;
        }
    }
    return 0;
}
