// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov121/castelia_rush.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The crowds of Castelia City's streets, started by script command 0x134 (CasteliaRushInit): up to 32 passers-by walk
// along the lanes of the zone's entry in archive 183, step around whoever stands in their way, some of them stop to
// talk to the player, and speech balloons of message file 209 pop up at random places of the bottom screen. The ROM
// embeds no name for this file, so its name is a guess after swan's name of its process, data_ov055_021f45f0
#include "types.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "field/castelia_rush.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/field_async_proc.h"

#define ARCID_CASTELIA_RUSH 186
// File 5 of archive 183: the zones that have a crowd, as u32s, in the order of their files
#define CASTELIA_RUSH_FILE_ZONES 5
// File 6 of archive 183: the scripts of the object codes, CasteliaRushScripts entries
#define CASTELIA_RUSH_FILE_SCRIPTS 6

#define PASSERBY_COUNT 32
#define PASSERBY_FIRST_ID 48
#define LANE_COUNT 2
#define BALLOON_COUNT 16
// How many frames a balloon stays up
#define BALLOON_FRAMES 16

// The script of a passer-by that talks to no one
#define SCRID_PASSERBY_NONE 2000

enum {
    PASSERBY_WALKER,
    // A passer-by who talks to the player when the player stands in its way
    PASSERBY_TALKER,
};

enum {
    PASSERBY_STATE_WAIT,
    PASSERBY_STATE_STEP,
    PASSERBY_STATE_END,
};

// A lane of a street: the passers-by start on one edge and walk in the lane's direction for its number of steps
typedef struct {
    u16 zMin;
    u16 zMax;
    u16 xMin;
    u16 xMax;
    u16 dir;
    u16 steps;
} CasteliaRushLane;

// The scripts that a passer-by of an object code picks from at random
typedef struct {
    u32 objCode;
    u16 scripts[6];
} CasteliaRushScripts;

// A zone's crowd, the first 0x32 bytes from its file of archive 183
// Black 2's; twinned code confirms the offsets of intervals, laneCount, objCodeCount, objCodes, msgBase, msgCount,
// stepFrames, lanes, laneTimers, scripts and scriptCount in Black
typedef struct {
    // The frames between passers-by, and the base of the frames between balloons, by day period
    s16 intervals[5];
    u16 unkA;
    u8 laneCount;
    u8 objCodeCount;
    u16 objCodes[4];
    // The balloons show messages msgBase to msgBase + msgCount - 1
    u8 msgBase;
    u8 msgCount;
    // The frames that a passer-by takes for a step
    u8 stepFrames;
    u8 unk19;
    CasteliaRushLane lanes[LANE_COUNT];
    // The frames since each lane's last passer-by
    s16 laneTimers[LANE_COUNT];
    CasteliaRushScripts *scripts;
    u32 scriptCount;
} CasteliaRushConfig;

typedef struct {
    u8 active;
    u8 state;
    u8 type;
    u8 dir;
    u16 steps;
    u16 scrId;
    u16 id;
    // The walking animation command of the passer-by's speed, for direction 0
    u16 walkAcmd;
    FieldActor *actor;
    const CasteliaRushLane *lane;
    // Whether the passer-by stepped aside last, and the direction it took
    u16 dodging;
    u16 dodgeDir;
} CasteliaRushPasserby;

typedef struct {
    u16 active;
    u8 x;
    u8 y;
    u16 index;
    s16 timer;
} CasteliaRushBalloon;

typedef struct {
    s16 timer;
    s16 interval;
    u32 next;
    MsgData *msgData;
    void *msgBGSys;
    CasteliaRushBalloon balloons[BALLOON_COUNT];
} CasteliaRushBalloons;

struct CasteliaRush {
    FieldPlayer *player;
    MMSys *actorSys;
    CasteliaRushPasserby passersby[PASSERBY_COUNT];
    CasteliaRushConfig config;
    CasteliaRushBalloons balloons;
};

typedef void (*PasserbyUpdateFunc)(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player);
typedef void (*PasserbyStartFunc)(CasteliaRushPasserby *passerby);

static void func_ov055_021f39c0(FieldAsyncProc *proc, Field *field, void *data);
static void func_ov055_021f3a64(FieldAsyncProc *proc, Field *field, void *data);
void func_ov055_021f3aa4(FieldAsyncProc *proc, Field *field, void *data);
static CasteliaRushPasserby *func_ov055_021f3b20(CasteliaRush *rush);
static CasteliaRushPasserby *func_ov055_021f3b48(CasteliaRush *rush, u16 x, u16 z, u16 dir, BOOL unused,
                                                 BOOL *occupied);
static void func_ov055_021f3c2c(CasteliaRushPasserby *passerby, MMSys *actorSys, u16 zoneId,
                                const CasteliaRushConfig *config, u32 index);
static void func_ov055_021f3ce4(CasteliaRushPasserby *passerby);
static void func_ov055_021f3cf4(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player);
static void func_ov055_021f3d0c(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player, u8 type, u16 x,
                                u16 z, u16 dir, u16 steps, const CasteliaRushLane *lane);
static void func_ov055_021f3d90(CasteliaRushPasserby *passerby);
static void func_ov055_021f3dbc(CasteliaRushPasserby *passerby);
static void func_ov055_021f3dd4(CasteliaRushPasserby *passerby, FieldActor *obstacle);
static void func_ov055_021f3e84(CasteliaRushConfig *config, u16 dayPeriod, u16 zoneId, HeapID heapId);
static void func_ov055_021f3f20(CasteliaRushConfig *config);
static void func_ov055_021f3f30(CasteliaRushConfig *config, CasteliaRush *rush, u32 dayPeriod);
static void func_ov055_021f3f9c(const CasteliaRush *rush, CasteliaRushConfig *config, const CasteliaRushLane *lane,
                                CasteliaRushPasserby *passerby, u16 offset);
static void func_ov055_021f405c(CasteliaRushConfig *config, CasteliaRush *rush, u16 dayPeriod);
static u16 func_ov055_021f40f4(const CasteliaRushConfig *config);
static void func_ov055_021f4124(CasteliaRushPasserby *passerby);
static void func_ov055_021f414c(CasteliaRushPasserby *passerby);
static void func_ov055_021f4174(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player);
static void func_ov055_021f41f0(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player);
static BOOL func_ov055_021f42ac(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldActor **found, u16 maxDist,
                                u16 minDist);
static BOOL func_ov055_021f4318(const CasteliaRushPasserby *passerby, FieldActor *actor, u16 maxDist, u16 minDist);
static BOOL func_ov055_021f4388(s16 x, s16 z, s16 otherX, s16 otherZ, u16 maxDist, u16 minDist, u16 dir);
static void func_ov055_021f43d8(CasteliaRushBalloons *balloons, void *msgBGSys, const CasteliaRushConfig *config,
                                u16 dayPeriod, HeapID heapId);
static void func_ov055_021f4430(CasteliaRushBalloons *balloons);
static void func_ov055_021f4458(CasteliaRushBalloons *balloons);
void func_ov055_021f4474(CasteliaRushBalloons *balloons, const CasteliaRushConfig *config, Field *field);
static void func_ov055_021f4510(CasteliaRushBalloon *balloon, u16 index);
static void func_ov055_021f4530(CasteliaRushBalloon *balloon, void *msgBGSys, MsgData *msgData, u32 msgBase,
                                u32 msgCount);
static void func_ov055_021f4574(CasteliaRushBalloon *balloon, void *msgBGSys);
static void func_ov055_021f459c(CasteliaRushBalloon *balloon);
static void func_ov055_021f45a4(CasteliaRushBalloon *balloon, void *msgBGSys);

// pokebw2's Passerby_UpdateTalker
// pokebw2's Passerby_UpdateWalker
// pokebw2's sPasserbyStartFuncs
static const PasserbyStartFunc data_ov055_021f45c8[] = { func_ov055_021f4124, func_ov055_021f414c };

// pokebw2's Passerby_StartTalker
// pokebw2's Passerby_StartWalker
// pokebw2's sPasserbyUpdateFuncs
static const PasserbyUpdateFunc data_ov055_021f45c0[] = { func_ov055_021f4174, func_ov055_021f41f0 };

// The balloons' places on the bottom screen, in tiles
// pokebw2's sBalloonX
static const u8 data_ov055_021f45e0[BALLOON_COUNT] = { 1, 22, 2, 20, 4, 21, 3, 19, 1, 19, 2, 22, 3, 20, 4, 21 };
// pokebw2's sBalloonY
static const u8 data_ov055_021f45d0[BALLOON_COUNT] = { 6, 2, 17, 14, 17, 10, 14, 2, 10, 17, 10, 6, 2, 6, 14, 6 };

// pokebw2's CASTELIA_RUSH_PROC_DEF
const FieldAsyncProcDef data_ov055_021f45f0 = {
    0, sizeof(CasteliaRush), func_ov055_021f39c0, func_ov055_021f3a64, func_ov055_021f3aa4, NULL,
};

// The passers-by's template, given each one's object code and id before it is used
// pokebw2's sPasserbyTemplate
static ZoneNPC data_ov055_021f4620 = { PASSERBY_FIRST_ID, 10 };

// pokebw2's CasteliaRush_Init
static void func_ov055_021f39c0(FieldAsyncProc *proc, Field *field, void *data) {
    CasteliaRush *rush = data;
    HeapID heapId;
    int i;
    void *msgBGSys = Field_GetMsgBGSys(field);
    GameData *gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    u16 zoneId;
    u16 dayPeriod;

    rush->player = Field_GetPlayer(field);
    rush->actorSys = Field_GetActorSystem(field);
    zoneId = Field_GetPlayerStateZoneID(field);
    dayPeriod = GameData_GetDayPeriod(gameData);
    heapId = Field_GetHeapID(field);

    func_ov055_021f3e84(&rush->config, dayPeriod, zoneId, heapId);
    for (i = 0; i < PASSERBY_COUNT; i++) {
        func_ov055_021f3c2c(&rush->passersby[i], rush->actorSys, zoneId, &rush->config, i);
    }
    func_ov055_021f405c(&rush->config, rush, dayPeriod);
    func_ov055_021f43d8(&rush->balloons, msgBGSys, &rush->config, dayPeriod, heapId);
    Field_SetCasteliaRush(field, rush);
}

// pokebw2's CasteliaRush_Free
static void func_ov055_021f3a64(FieldAsyncProc *proc, Field *field, void *data) {
    CasteliaRush *rush = data;
    int i;

    Field_SetCasteliaRush(field, NULL);
    for (i = 0; i < PASSERBY_COUNT; i++) {
        func_ov055_021f3ce4(&rush->passersby[i]);
    }
    func_ov055_021f3f20(&rush->config);
    func_ov055_021f4430(&rush->balloons);
    rush->player = NULL;
    rush->actorSys = NULL;
}

static u32 Field_IsEventRunning(Field *field);
void func_ov055_021f3aa4(FieldAsyncProc *proc, Field *field, void *data) {
    CasteliaRush *rush = data;
    int i;

    func_ov055_021f3f30(&rush->config, rush, Field_GetDayPeriod(field));
    if (Field_IsEventRunning(field)) {
        for (i = 0; i < PASSERBY_COUNT; i++) {
            func_ov055_021f3d90(&rush->passersby[i]);
        }
    }
    for (i = 0; i < PASSERBY_COUNT; i++) {
        func_ov055_021f3cf4(&rush->passersby[i], rush->actorSys, rush->player);
    }
    func_ov055_021f4474(&rush->balloons, &rush->config, field);
}


// pokebw2's CasteliaRush_ClearBalloons
void func_ov055_021f3b10(CasteliaRush *rush) {
    func_ov055_021f4458(&rush->balloons);
}

// pokebw2's CasteliaRush_FindFreePasserby
static CasteliaRushPasserby *func_ov055_021f3b20(CasteliaRush *rush) {
    int i;

    for (i = 0; i < PASSERBY_COUNT; i++) {
        if (!rush->passersby[i].active) {
            return &rush->passersby[i];
        }
    }
    return NULL;
}

// The talker that stands on (x, z), setting *occupied, or the first talker in the direction dir from it
static CasteliaRushPasserby *func_ov055_021f3b48(CasteliaRush *rush, u16 x, u16 z, u16 dir, BOOL unused,
                                                 BOOL *occupied) {
    int i;
    s16 dx;
    s16 dz;
    s16 dirX;
    s16 dirZ;

    switch (dir) {
    case 0:
        dirX = 0;
        dirZ = -1;
        break;
    case 1:
        dirX = 0;
        dirZ = 1;
        break;
    case 3:
        dirX = 1;
        dirZ = 0;
        break;
    case 2:
        dirX = -1;
        dirZ = 0;
        break;
    }

    *occupied = FALSE;
    for (i = 0; i < PASSERBY_COUNT; i++) {
        if (rush->passersby[i].active && rush->passersby[i].type == PASSERBY_TALKER) {
            s16 actorX = GetGPosX(rush->passersby[i].actor);
            s16 actorZ = GetGPosZ(rush->passersby[i].actor);

            dx = actorX - x;
            dz = actorZ - z;
            if (dx != 0) {
                dx = dx / (dx < 0 ? -dx : dx);
            }
            if (dz != 0) {
                dz = dz / (dz < 0 ? -dz : dz);
            }
            if (dx == 0 && dz == 0) {
                *occupied = TRUE;
                return &rush->passersby[i];
            }
            if (dx == dirX && dz == dirZ) {
                return &rush->passersby[i];
            }
        }
    }
    return NULL;
}

static void func_ov055_021f3c2c(CasteliaRushPasserby *passerby, MMSys *actorSys, u16 zoneId,
                                const CasteliaRushConfig *config, u32 index) {
    u32 i = 0;
    u16 objCode;
    u32 rand;

    objCode = config->objCodes[GFL_RandomLC(0) % config->objCodeCount];
    data_ov055_021f4620.modelId = objCode;
    rand = GFL_RandomLC(0);
    passerby->id = index + PASSERBY_FIRST_ID;
    for (i = 0; i < config->scriptCount; i++) {
        if (objCode == config->scripts[i].objCode) {
            passerby->scrId = config->scripts[i].scripts[rand % 6];
            break;
        }
    }

    passerby->actor = CreateNewActorByEntityNoWKOBJCODE(actorSys, &data_ov055_021f4620, zoneId);
    passerby->active = FALSE;
    passerby->state = PASSERBY_STATE_WAIT;
    passerby->type = PASSERBY_WALKER;
    passerby->dir = 0;
    passerby->walkAcmd = func_ov055_021f40f4(config);
    SetActorHidden(passerby->actor, TRUE);
    func_ov010_0216dccc(passerby->actor, TRUE);

    {
        VecFx32 offset = { 0, 0, 0 };

        offset.x = GFL_RandomLC(FX32_ONE * 8) - FX32_ONE * 4;
        SetActorWPosOffset(passerby->actor, &offset);
    }
}

// pokebw2's Passerby_Free
static void func_ov055_021f3ce4(CasteliaRushPasserby *passerby) {
    DeleteActor(passerby->actor);
    passerby->actor = NULL;
}

// pokebw2's Passerby_Update
static void func_ov055_021f3cf4(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player) {
    if (passerby->active) {
        data_ov055_021f45c0[passerby->type](passerby, actorSys, player);
    }
}

static void func_ov055_021f3d0c(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player, u8 type, u16 x,
                                u16 z, u16 dir, u16 steps, const CasteliaRushLane *lane) {
    ZoneNPC npc;

    passerby->active = TRUE;
    SetActorHidden(passerby->actor, FALSE);
    npc = data_ov055_021f4620;
    npc.modelId = FldAct_GetObjCode(passerby->actor);
    npc.direction = dir;
    npc.uid = passerby->id;
    func_ov010_0216e618(&npc, x, z, 0);
    FldAct_Transplant(passerby->actor, &npc);
    passerby->type = type;
    passerby->dir = dir;
    passerby->state = PASSERBY_STATE_WAIT;
    passerby->steps = steps;
    passerby->dodging = FALSE;
    passerby->dodgeDir = 0;
    func_ov010_0216dccc(passerby->actor, TRUE);
    passerby->lane = lane;
    data_ov055_021f45c8[type](passerby);
}

// A talker that isn't the one talking to the player walks on while an event runs
// pokebw2's Passerby_StopTalker
static void func_ov055_021f3d90(CasteliaRushPasserby *passerby) {
    if (passerby->active == TRUE && passerby->type == PASSERBY_TALKER &&
        FldAct_GetSCRID(passerby->actor) == SCRID_PASSERBY_NONE) {
        passerby->type = PASSERBY_WALKER;
        func_ov055_021f4124(passerby);
    }
}

// pokebw2's Passerby_End
static void func_ov055_021f3dbc(CasteliaRushPasserby *passerby) {
    passerby->active = FALSE;
    passerby->type = PASSERBY_WALKER;
    passerby->dir = 0;
    passerby->state = PASSERBY_STATE_WAIT;
    SetActorHidden(passerby->actor, TRUE);
}

// Steps aside from an actor in the way, keeping the direction of the last step aside, and turning back at the lane's
// edges
// pokebw2's Passerby_StepAside
static void func_ov055_021f3dd4(CasteliaRushPasserby *passerby, FieldActor *obstacle) {
    u16 dir = GetActorFaceDir(obstacle);
    s16 x = GetGPosX(passerby->actor);
    s16 z = GetGPosZ(passerby->actor);

    if (passerby->dodging) {
        dir = passerby->dodgeDir;
    } else if (dir == passerby->dir || GetInverseDirection(dir) == passerby->dir) {
        if (GFL_RandomLC(2) == 0) {
            dir = dir + 2;
        } else {
            dir = GetInverseDirection((u16)(dir + 2));
        }
    } else {
        dir = GetInverseDirection(dir);
    }

    x += GetDirectionVectorCompX(dir);
    z = z + GetDirectionVectorCompZ(dir);
    if (dir <= 1) {
        if (z < passerby->lane->zMin || z > passerby->lane->zMax) {
            dir = GetInverseDirection(dir);
        }
    } else {
        if (x < passerby->lane->xMin || x > passerby->lane->xMax) {
            dir = GetInverseDirection(dir);
        }
    }
    FldAct_SetAcmd(passerby->actor, passerby->walkAcmd + dir);
    passerby->dodging = TRUE;
    passerby->dodgeDir = dir;
}

// pokebw2's CasteliaRushConfig_Load
static void func_ov055_021f3e84(CasteliaRushConfig *config, u16 dayPeriod, u16 zoneId, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_CASTELIA_RUSH, HEAPID_TAIL(heapId));
    u32 *zones = GFL_ArcToolReadHeapNew(arc, CASTELIA_RUSH_FILE_ZONES, HEAPID_TAIL(heapId));
    u32 count = GFL_ArcToolGetDataLength(arc, CASTELIA_RUSH_FILE_ZONES) / sizeof(u32);
    u32 file;
    u32 size;
    int i;

    for (file = 0; file < count; file++) {
        if (zoneId == zones[file]) {
            break;
        }
    }
    GFL_ArcToolReadRange(arc, file, 0, 0x32, config);
    GFL_HeapFree(zones);

    size = GFL_ArcToolGetDataLength(arc, CASTELIA_RUSH_FILE_SCRIPTS);
    config->scripts = GFL_ArcToolReadHeapNew(arc, CASTELIA_RUSH_FILE_SCRIPTS, heapId);
    config->scriptCount = size / sizeof(CasteliaRushScripts);
    GFL_ArcToolFree(arc);

    for (i = 0; i < LANE_COUNT; i++) {
        config->laneTimers[i] = 0;
    }
}

// pokebw2's CasteliaRushConfig_Free
static void func_ov055_021f3f20(CasteliaRushConfig *config) {
    GFL_HeapFree(config->scripts);
    config->scripts = NULL;
}

// pokebw2's CasteliaRush_SpawnPassersby
static void func_ov055_021f3f30(CasteliaRushConfig *config, CasteliaRush *rush, u32 dayPeriod) {
    int i;

    for (i = 0; i < config->laneCount; i++) {
        if (config->laneTimers[i] >= config->intervals[dayPeriod]) {
            CasteliaRushPasserby *passerby = func_ov055_021f3b20(rush);

            if (passerby != NULL) {
                func_ov055_021f3f9c(rush, config, &config->lanes[i], passerby, 0);
                config->laneTimers[i] = 0;
            }
        } else {
            config->laneTimers[i]++;
        }
    }
}

// Starts a passer-by offset steps into a lane, at a random place across it, unless a talker stands there
static void func_ov055_021f3f9c(const CasteliaRush *rush, CasteliaRushConfig *config, const CasteliaRushLane *lane,
                                CasteliaRushPasserby *passerby, u16 offset) {
    u16 x;
    u16 z;
    u8 talker = FALSE;
    u32 rand = GFL_RandomLC(0);
    u16 dir = lane->dir;
    BOOL occupied;

    if (dir <= 1) {
        x = lane->xMin + rand % (lane->xMax - lane->xMin + 1);
        if (dir == 0) {
            z = lane->zMin - offset;
        } else {
            z = lane->zMin + offset;
        }
    } else {
        if (dir == 2) {
            x = lane->xMin - offset;
        } else {
            x = lane->xMin + offset;
        }
        z = lane->zMin + rand % (lane->zMax - lane->zMin + 1);
    }

    if (func_ov055_021f3b48(rush, x, z, dir, TRUE, &occupied) == NULL && rand % 10 == 0) {
        talker = TRUE;
    }
    if (!occupied) {
        func_ov055_021f3d0c(passerby, rush->actorSys, rush->player, talker, x, z, lane->dir, lane->steps - offset,
                            lane);
    }
}

// Fills the lanes with passers-by as if they had been walking for a while
// pokebw2's CasteliaRush_FillLanes
static void func_ov055_021f405c(CasteliaRushConfig *config, CasteliaRush *rush, u16 dayPeriod) {
    int i;
    int j;
    int frames;
    int count;
    CasteliaRushPasserby *passerby;

    for (i = 0; i < config->laneCount; i++) {
        count = config->lanes[i].steps * 6 / config->intervals[dayPeriod];
        frames = 0;
        for (j = 0; j < count; j++) {
            passerby = func_ov055_021f3b20(rush);
            if (passerby != NULL) {
                func_ov055_021f3f9c(rush, config, &config->lanes[i], passerby, frames / 6);
                frames += config->intervals[dayPeriod];
            }
        }
    }
}

// pokebw2's CasteliaRushConfig_GetWalkAcmd
static u16 func_ov055_021f40f4(const CasteliaRushConfig *config) {
    if (config->stepFrames == 2) {
        return 20;
    }
    if (config->stepFrames == 4) {
        return 16;
    }
    if (config->stepFrames == 6) {
        return 76;
    }
    if (config->stepFrames == 8) {
        return 12;
    }
    if (config->stepFrames == 16) {
        return 8;
    }
    return 12;
}

static void func_ov055_021f4124(CasteliaRushPasserby *passerby) {
    func_ov010_0216db4c(passerby->actor, TRUE);
    func_ov010_0216dc98(passerby->actor, TRUE);
    func_ov010_0216dae0(passerby->actor, FALSE);
    SetActorSCRID(passerby->actor, SCRID_PASSERBY_NONE);
}

static void func_ov055_021f414c(CasteliaRushPasserby *passerby) {
    func_ov010_0216db4c(passerby->actor, TRUE);
    func_ov010_0216dc98(passerby->actor, TRUE);
    func_ov010_0216dae0(passerby->actor, TRUE);
    SetActorSCRID(passerby->actor, SCRID_PASSERBY_NONE);
}

static void func_ov055_021f4174(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player) {
    FieldActor *obstacle;

    switch (passerby->state) {
    case PASSERBY_STATE_WAIT:
        if (!func_ov010_0216d454(passerby->actor)) {
            break;
        }
        passerby->state++;
    case PASSERBY_STATE_STEP:
        if (passerby->steps == 0) {
            passerby->state = PASSERBY_STATE_END;
            return;
        }
        if (func_ov055_021f42ac(passerby, actorSys, &obstacle, 2, 0)) {
            func_ov055_021f3dd4(passerby, obstacle);
        } else {
            FldAct_SetAcmd(passerby->actor, passerby->walkAcmd + passerby->dir);
            passerby->steps--;
        }
        passerby->state = PASSERBY_STATE_WAIT;
        break;
    case PASSERBY_STATE_END:
        func_ov055_021f3dbc(passerby);
        break;
    }
}

static void func_ov055_021f41f0(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldPlayer *player) {
    FieldActor *obstacle;

    switch (passerby->state) {
    case PASSERBY_STATE_WAIT:
        if (!func_ov010_0216d454(passerby->actor)) {
            break;
        }
        passerby->state++;
    case PASSERBY_STATE_STEP:
        if (passerby->steps == 0) {
            passerby->state = PASSERBY_STATE_END;
            return;
        }
        if (func_ov055_021f4318(passerby, FieldPlayer_GetActor(player), 1, 1)) {
            SetActorSCRID(passerby->actor, passerby->scrId);
            return;
        }
        if (func_ov055_021f42ac(passerby, actorSys, &obstacle, 1, 0)) {
            func_ov055_021f3dd4(passerby, obstacle);
            SetActorSCRID(passerby->actor, SCRID_PASSERBY_NONE);
            func_ov055_021f3d90(passerby);
        } else {
            FldAct_SetAcmd(passerby->actor, passerby->walkAcmd + passerby->dir);
            passerby->steps--;
            SetActorSCRID(passerby->actor, SCRID_PASSERBY_NONE);
        }
        passerby->state = PASSERBY_STATE_WAIT;
        break;
    case PASSERBY_STATE_END:
        func_ov055_021f3dbc(passerby);
        break;
    }
}

// The first other visible actor ahead of the passer-by
static BOOL func_ov055_021f42ac(CasteliaRushPasserby *passerby, MMSys *actorSys, FieldActor **found, u16 maxDist,
                                u16 minDist) {
    u32 index = 0;
    FieldActor *actor;

    while (NextActor(actorSys, &actor, &index) == TRUE) {
        if (passerby->actor != actor && !CheckActorFlag(actor, 0x80) && !func_ov010_0216dab4(actor) &&
            func_ov055_021f4318(passerby, actor, maxDist, minDist)) {
            *found = actor;
            return TRUE;
        }
    }
    return FALSE;
}

// Whether the actor or its starting place is ahead of the passer-by, between minDist and maxDist tiles
// pokebw2's Passerby_IsActorAhead
static BOOL func_ov055_021f4318(const CasteliaRushPasserby *passerby, FieldActor *actor, u16 maxDist, u16 minDist) {
    s16 actorX = GetGPosX(actor);
    s16 actorZ = GetGPosZ(actor);
    s16 x = GetGPosX(passerby->actor);
    s16 z = GetGPosZ(passerby->actor);

    if (func_ov055_021f4388(x, z, actorX, actorZ, maxDist, minDist, passerby->dir)) {
        return TRUE;
    }
    return func_ov055_021f4388(x, z, FldAct_GetInitGPosX(actor), FldAct_GetInitGPosZ(actor), maxDist, minDist,
                               passerby->dir);
}

// pokebw2's IsPosAhead
static BOOL func_ov055_021f4388(s16 x, s16 z, s16 otherX, s16 otherZ, u16 maxDist, u16 minDist, u16 dir) {
    int ahead;
    int side;

    switch (dir) {
    case 0:
        ahead = z - otherZ;
        side = x - otherX;
        break;
    case 1:
        ahead = otherZ - z;
        side = x - otherX;
        break;
    case 2:
        ahead = x - otherX;
        side = z - otherZ;
        break;
    case 3:
        ahead = otherX - x;
        side = z - otherZ;
        break;
    }

    if (ahead <= maxDist && ahead >= minDist && side == 0) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov055_021f43d8(CasteliaRushBalloons *balloons, void *msgBGSys, const CasteliaRushConfig *config,
                                u16 dayPeriod, HeapID heapId) {
    int i = 0;
    u32 rand = GFL_RandomLC(0);

    balloons->timer = 0;
    balloons->interval = rand % config->intervals[dayPeriod] + BALLOON_FRAMES;
    balloons->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10350, heapId);
    balloons->msgBGSys = msgBGSys;
    balloons->next = rand % BALLOON_COUNT;
    for (i = 0; i < BALLOON_COUNT; i++) {
        func_ov055_021f4510(&balloons->balloons[i], i);
    }
}

// pokebw2's CasteliaRushBalloons_Free
static void func_ov055_021f4430(CasteliaRushBalloons *balloons) {
    int i;

    for (i = 0; i < BALLOON_COUNT; i++) {
        func_ov055_021f45a4(&balloons->balloons[i], balloons->msgBGSys);
    }
    balloons->msgBGSys = NULL;
    GFL_MsgDataFree(balloons->msgData);
    balloons->msgData = NULL;
}

// pokebw2's CasteliaRushBalloons_Clear
static void func_ov055_021f4458(CasteliaRushBalloons *balloons) {
    int i;

    for (i = 0; i < BALLOON_COUNT; i++) {
        func_ov055_021f45a4(&balloons->balloons[i], balloons->msgBGSys);
    }
}

void func_ov055_021f4474(CasteliaRushBalloons *balloons, const CasteliaRushConfig *config, Field *field) {
    int i = 0;
    u32 rand = GFL_RandomLC(0);
    int dayPeriod = Field_GetDayPeriod(field);

    if (!Field_IsEventRunning(field)) {
        balloons->timer++;
        if (balloons->timer >= balloons->interval) {
            func_ov055_021f4530(&balloons->balloons[balloons->next], balloons->msgBGSys, balloons->msgData,
                                config->msgBase, config->msgCount);
            balloons->timer = 0;
            balloons->next = (balloons->next + 1) % BALLOON_COUNT;
            balloons->interval = rand % config->intervals[dayPeriod] + BALLOON_FRAMES;
        }
        for (i = 0; i < BALLOON_COUNT; i++) {
            func_ov055_021f4574(&balloons->balloons[i], balloons->msgBGSys);
        }
    } else {
        for (i = 0; i < BALLOON_COUNT; i++) {
            func_ov055_021f45a4(&balloons->balloons[i], balloons->msgBGSys);
        }
    }
}


// pokebw2's CasteliaRushBalloon_Init
static void func_ov055_021f4510(CasteliaRushBalloon *balloon, u16 index) {
    balloon->active = FALSE;
    balloon->x = data_ov055_021f45e0[index];
    balloon->y = data_ov055_021f45d0[index];
    balloon->index = index;
    balloon->timer = 0;
}

static void func_ov055_021f4530(CasteliaRushBalloon *balloon, void *msgBGSys, MsgData *msgData, u32 msgBase,
                                u32 msgCount) {
    u32 rand = GFL_RandomLC(0);

    balloon->active = TRUE;
    balloon->timer = 0;
    func_ov021_02191870(msgBGSys, msgData, msgBase + rand % msgCount, balloon->index, balloon->x, balloon->y, 8, 2);
}

// pokebw2's CasteliaRushBalloon_Update
static void func_ov055_021f4574(CasteliaRushBalloon *balloon, void *msgBGSys) {
    if (balloon->active) {
        balloon->timer++;
        if (balloon->timer >= BALLOON_FRAMES) {
            func_ov021_0219196c(msgBGSys, balloon->index);
            func_ov055_021f459c(balloon);
        }
    }
}

// pokebw2's CasteliaRushBalloon_Reset
static void func_ov055_021f459c(CasteliaRushBalloon *balloon) {
    balloon->timer = 0;
    balloon->active = FALSE;
}

// pokebw2's CasteliaRushBalloon_Hide
static void func_ov055_021f45a4(CasteliaRushBalloon *balloon, void *msgBGSys) {
    if (balloon->active) {
        func_ov021_0219196c(msgBGSys, balloon->index);
        func_ov055_021f459c(balloon);
    }
}
