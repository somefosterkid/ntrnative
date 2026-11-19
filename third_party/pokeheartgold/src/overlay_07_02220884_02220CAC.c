#include "global.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_generic_emitter.h"

struct BattleAnimSystem;

struct GenericEmitterCallbackData;

struct ParticleSystem;

struct SPLEmitter;

struct SPLEmitterState;

struct SPLParticle;

struct SPLParticleList;

struct SPLResource;

enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};

typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);

typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;

static inline void SPLEmitter_SetAxis(SPLEmitter *emtr, const VecFx16 *axis) {
    emtr->axis = *axis;
}

BOOL GF3dGfxRawResObj_GetTex4x4Key(BattleAnimSystem *system);

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

int BattleAnimUtil_GetBattlerType(BattleAnimSystem *system, int battler);

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

extern const VecFx32 ov07_02235CE0[6][6];

extern const VecFx32 ov07_02235620[6][6];

extern const VecFx32 ov07_022357D0[6][6];

extern const VecFx32 ov07_02235B30[6][6];

extern const VecFx32 ov07_02235E90[6][6];

extern const VecFx32 ov07_02235980[6][6];

extern const VecFx32 ov07_02236040[6][6];

extern const VecFx32 ov07_022361F0[6][6];

void ov07_02220CAC(SPLEmitter *emitter, GenericEmitterCallbackData *data);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

BOOL ov07_0221BFC0(BattleAnimSystem *system);

typedef struct {
    int a[7][4];
} ov07_022355B0_Array;

extern const ov07_022355B0_Array ov07_022355B0;

typedef struct {
    int a[4];
} ov07_022354D0_Array;

extern const ov07_022354D0_Array ov07_022354D0;

typedef struct {
    int a[6][2];
} ov07_02235510_Array;

extern const ov07_02235510_Array ov07_02235510;

void ov07_02220CAC(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    VecFx16 axis;

    int st = ov07_02231924(data->battleAnimSys, data->startBattler);
    int et = ov07_02231924(data->battleAnimSys, data->endBattler);

    switch (data->params[GENERIC_EMITTER_CALLBACK_PARAM_AXIS]) {
    case 0:
        VEC_Fx16Set(&axis, 0, 0, 0);
        break;
    case 1:
    case 2:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_TARGET_MODE] == 2) {
                VEC_Fx16Set(&axis, ((fx16)(((-0.8145) > 0) ? (fx16)((-0.8145) * (1 << 12) + 0.5f) : (fx16)((-0.8145) * (1 << 12) - 0.5f))), ((fx16)(((0.4492) > 0) ? (fx16)((0.4492) * (1 << 12) + 0.5f) : (fx16)((0.4492) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
            } else {
                VEC_Fx16Set(&axis, ((fx16)(((0.5703) > 0) ? (fx16)((0.5703) * (1 << 12) + 0.5f) : (fx16)((0.5703) * (1 << 12) - 0.5f))), ((fx16)(((-0.3516) > 0) ? (fx16)((-0.3516) * (1 << 12) + 0.5f) : (fx16)((-0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
            }
        } else {
            VEC_Fx16Set(
                &axis,
                ov07_02235CE0[st][et].x,
                ov07_02235CE0[st][et].y,
                ov07_02235CE0[st][et].z / 2);
        }
        break;
    case 4:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, -ov07_02235CE0[st][et].x, ov07_02235CE0[st][et].y, ov07_02235CE0[st][et].z / 2);
        } else {
            if (ov07_0223192C(data->battleAnimSys, data->startBattler) == ov07_0223192C(data->battleAnimSys, data->endBattler)) {
                VEC_Fx16Set(
                    &axis,
                    ov07_02235CE0[st][et].x,
                    ov07_02235CE0[st][et].y,
                    ov07_02235CE0[st][et].z / 2);
            } else if (ov07_0223192C(data->battleAnimSys, data->startBattler) == BTLSCR_PLAYER) {
                VEC_Fx16Set(
                    &axis,
                    3776,
                    2112,
                    1532);
            } else {
                VEC_Fx16Set(
                    &axis,
                    -4228,
                    -2728,
                    1532);
            }
        }
        break;
    case 5:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, -ov07_02235CE0[st][et].x, ov07_02235CE0[st][et].y, ov07_02235CE0[st][et].z / 2);
        } else {
            if (ov07_0223192C(data->battleAnimSys, data->startBattler) == ov07_0223192C(data->battleAnimSys, data->endBattler)) {
                VEC_Fx16Set(
                    &axis,
                    ov07_02235CE0[st][et].x,
                    ov07_02235CE0[st][et].y,
                    ov07_02235CE0[st][et].z / 2);
            } else if (ov07_0223192C(data->battleAnimSys, data->startBattler) == BTLSCR_PLAYER) {
                VEC_Fx16Set(
                    &axis,
                    3776,
                    2112,
                    1532);
            } else {
                VEC_Fx16Set(
                    &axis,
                    -4228,
                    -2728,
                    1532);
            }
        }
        break;
    case 8:
    case 9:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_TARGET_MODE] == 2) {
                VEC_Fx16Set(&axis, ((fx16)(((-0.5703) > 0) ? (fx16)((-0.5703) * (1 << 12) + 0.5f) : (fx16)((-0.5703) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
            } else {
                VEC_Fx16Set(&axis, ((fx16)(((0.5703) > 0) ? (fx16)((0.5703) * (1 << 12) + 0.5f) : (fx16)((0.5703) * (1 << 12) - 0.5f))), ((fx16)(((-0.3516) > 0) ? (fx16)((-0.3516) * (1 << 12) + 0.5f) : (fx16)((-0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
            }
        } else {
            VEC_Fx16Set(&axis, ov07_02235620[st][et].x, ov07_02235620[st][et].y, ov07_02235620[st][et].z / 2);
        }
        break;
    case 10:
    case 11:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-0.5703) > 0) ? (fx16)((-0.5703) * (1 << 12) + 0.5f) : (fx16)((-0.5703) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {

            VEC_Fx16Set(&axis, ov07_022357D0[st][et].x, ov07_02235620[st][et].y, ov07_02235620[st][et].z / 2);
        }
        break;
    case 12:
    case 13:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-0.7168) > 0) ? (fx16)((-0.7168) * (1 << 12) + 0.5f) : (fx16)((-0.7168) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ov07_02235B30[st][et].x, ov07_02235B30[st][et].y, ov07_02235B30[st][et].z);
        }
        break;
    case 14:
    case 15:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-0.5703) > 0) ? (fx16)((-0.5703) * (1 << 12) + 0.5f) : (fx16)((-0.5703) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ov07_02235E90[st][et].x, ov07_02235E90[st][et].y, ov07_02235E90[st][et].z);
        }
        break;
    case 16:
    case 17:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-0.5703) > 0) ? (fx16)((-0.5703) * (1 << 12) + 0.5f) : (fx16)((-0.5703) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ov07_02235980[st][et].x, ov07_02235980[st][et].y, ov07_02235980[st][et].z);
        }
        break;
    case 18:
    case 19:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-0.5703) > 0) ? (fx16)((-0.5703) * (1 << 12) + 0.5f) : (fx16)((-0.5703) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ov07_02236040[st][et].x, ov07_02236040[st][et].y, ov07_02236040[st][et].z);
        }
        break;
    case 20:
    case 21:
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            VEC_Fx16Set(&axis, ((fx16)(((-1.0586) > 0) ? (fx16)((-1.0586) * (1 << 12) + 0.5f) : (fx16)((-1.0586) * (1 << 12) - 0.5f))), ((fx16)(((0.3516) > 0) ? (fx16)((0.3516) * (1 << 12) + 0.5f) : (fx16)((0.3516) * (1 << 12) - 0.5f))), ((fx16)(((0.1797) > 0) ? (fx16)((0.1797) * (1 << 12) + 0.5f) : (fx16)((0.1797) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ov07_022361F0[st][et].x, ov07_022361F0[st][et].y, ov07_022361F0[st][et].z);
        }
        break;
    case 6:
    case 7: {
        int i;
        int p_e[4];
        int p1_e1[4];
        int p1_p2[4];
        int p1_e2[4];
        int p2_p1[4];
        int p2_e1[4];
        int p2_e2[4];
        ov07_022355B0_Array axisConfigs = ov07_022355B0;

        ov07_022354D0_Array contestConfig = ov07_022354D0;

        for (i = 0; i < 4; i++) {
            p_e[i] = axisConfigs.a[0][i];
            p1_e1[i] = axisConfigs.a[1][i];
            p1_p2[i] = axisConfigs.a[2][i];
            p1_e2[i] = axisConfigs.a[3][i];
            p2_p1[i] = axisConfigs.a[4][i];
            p2_e1[i] = axisConfigs.a[5][i];
            p2_e2[i] = axisConfigs.a[6][i];
        }

        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            for (i = 0; i < 4; i++) {
                p_e[i] = contestConfig.a[i];
                p1_e1[i] = contestConfig.a[i];
                p1_p2[i] = contestConfig.a[i];
                p1_e2[i] = contestConfig.a[i];
                p2_p1[i] = contestConfig.a[i];
                p2_e1[i] = contestConfig.a[i];
                p2_e2[i] = contestConfig.a[i];
            }
        }

        switch (st) {
        case 0:
        default:
            VEC_Fx16Set(&axis, p_e[0], p_e[1], p_e[2]);
            break;
        case 1:
            if (p_e[3] == 1) {
                VEC_Fx16Set(&axis, p_e[0], p_e[1], p_e[2]);
            } else {
                VEC_Fx16Set(&axis, -p_e[0], -p_e[1], -p_e[2]);
            }
            break;
        case 2:
            if (et == 3) {
                VEC_Fx16Set(&axis, p1_e1[0], p1_e1[1], p1_e1[2]);
            } else if (et == 5) {
                VEC_Fx16Set(&axis, p1_e2[0], p1_e2[1], p1_e2[2]);
            } else {
                VEC_Fx16Set(&axis, p1_p2[0], p1_p2[1], p1_p2[2]);
            }
            break;
        case 3:
            if (et == 2) {
                if (p1_e1[3] == 1) {
                    VEC_Fx16Set(&axis, p1_e1[0], p1_e1[1], p1_e1[2]);
                } else {
                    VEC_Fx16Set(&axis, -p1_e1[0], -p1_e1[1], -p1_e1[2]);
                }
            } else if (et == 5) {
                if (p1_p2[3] == 1) {
                    VEC_Fx16Set(&axis, -p2_p1[0], -p2_p1[1], -p2_p1[2]);
                } else {
                    VEC_Fx16Set(&axis, p2_p1[0], p2_p1[1], p2_p1[2]);
                }
            } else {
                if (p2_e1[3] == 1) {
                    VEC_Fx16Set(&axis, p2_e1[0], p2_e1[1], p2_e1[2]);
                } else {
                    VEC_Fx16Set(&axis, -p2_e1[0], -p2_e1[1], -p2_e1[2]);
                }
            }
            break;
        case 4:
            if (et == 3) {
                VEC_Fx16Set(&axis, p2_e1[0], p2_e1[1], p2_e1[2]);
            } else if (et == 5) {
                VEC_Fx16Set(&axis, p2_e2[0], p2_e2[1], p2_e2[2]);
            } else {
                VEC_Fx16Set(&axis, p2_p1[0], p2_p1[1], p2_p1[2]);
            }
            break;
        case 5:
            if (et == 3) {
                if (p1_e1[3] == 1) {
                    VEC_Fx16Set(&axis, p2_p1[0], p2_p1[1], p2_p1[2]);
                } else {
                    VEC_Fx16Set(&axis, -p2_p1[0], -p2_p1[1], -p2_p1[2]);
                }
            } else if (et == 2) {
                if (p1_e1[3] == 1) {
                    VEC_Fx16Set(&axis, p1_e2[0], p1_e2[1], p1_e2[2]);
                } else {
                    VEC_Fx16Set(&axis, -p1_e2[0], -p1_e2[1], -p1_e2[2]);
                }
            } else {
                if (p1_e1[3] == 1) {
                    VEC_Fx16Set(&axis, p2_e2[0], p2_e2[1], p2_e2[2]);
                } else {
                    VEC_Fx16Set(&axis, -p2_e2[0], -p2_e2[1], -p2_e2[2]);
                }
            }
            break;
        }
    } break;
    case 3:
        VEC_Fx16Set(&axis, ((fx32)(((-0.1953) > 0) ? ((-0.1953) * (1 << 12) + 0.5f) : ((-0.1953) * (1 << 12) - 0.5f))), ((fx32)(((0.293) > 0) ? ((0.293) * (1 << 12) + 0.5f) : ((0.293) * (1 << 12) - 0.5f))), ((fx32)(((0.122) > 0) ? ((0.122) * (1 << 12) + 0.5f) : ((0.122) * (1 << 12) - 0.5f))));
        break;
    case 22:
        VEC_Fx16Set(&axis, ((fx32)(((-0.8325) > 0) ? ((-0.8325) * (1 << 12) + 0.5f) : ((-0.8325) * (1 << 12) - 0.5f))), ((fx32)(((-0.6455) > 0) ? ((-0.6455) * (1 << 12) + 0.5f) : ((-0.6455) * (1 << 12) - 0.5f))), ((fx32)(((0.0) > 0) ? ((0.0) * (1 << 12) + 0.5f) : ((0.0) * (1 << 12) - 0.5f))));
        break;
    case 25:
        VEC_Fx16Set(&axis, ((fx32)(((-0.8398) > 0) ? ((-0.8398) * (1 << 12) + 0.5f) : ((-0.8398) * (1 << 12) - 0.5f))), ((fx32)(((0.4766) > 0) ? ((0.4766) * (1 << 12) + 0.5f) : ((0.4766) * (1 << 12) - 0.5f))), ((fx32)(((0.0) > 0) ? ((0.0) * (1 << 12) + 0.5f) : ((0.0) * (1 << 12) - 0.5f))));
        break;
    case 26:
        if (ov07_0223192C(data->battleAnimSys, data->startBattler) == BTLSCR_PLAYER) {
            VEC_Fx16Set(&axis, ((fx32)(((0.9219) > 0) ? ((0.9219) * (1 << 12) + 0.5f) : ((0.9219) * (1 << 12) - 0.5f))), ((fx32)(((0.5156) > 0) ? ((0.5156) * (1 << 12) + 0.5f) : ((0.5156) * (1 << 12) - 0.5f))), ((fx32)(((0.0) > 0) ? ((0.0) * (1 << 12) + 0.5f) : ((0.0) * (1 << 12) - 0.5f))));
        } else {
            VEC_Fx16Set(&axis, ((fx32)(((-1.4648) > 0) ? ((-1.4648) * (1 << 12) + 0.5f) : ((-1.4648) * (1 << 12) - 0.5f))), ((fx32)(((-0.537) > 0) ? ((-0.537) * (1 << 12) + 0.5f) : ((-0.537) * (1 << 12) - 0.5f))), ((fx32)(((0.0) > 0) ? ((0.0) * (1 << 12) + 0.5f) : ((0.0) * (1 << 12) - 0.5f))));
        }
        break;
    case 24: {
        int startType;
        ov07_02235510_Array axisConfigs = ov07_02235510;

        startType = ov07_02231924(data->battleAnimSys, data->startBattler);
        VEC_Fx16Set(&axis, axisConfigs.a[startType][0], axisConfigs.a[startType][1], 0);
    } break;
    }

    SPLEmitter_SetAxis(emitter, &axis);
}
