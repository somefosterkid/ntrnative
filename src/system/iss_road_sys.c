// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/iss_road_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/player_state.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "system/iss_road_sys.h"

// The interactive sound system's routes. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except
// func_0202b1ec, func_0202b204 and func_0202b224

// The BGM's tracks that the route sets the volume of
#define ISS_ROAD_TRACK_MASK ((1 << 8) | (1 << 9))
// The volume's change per frame
#define ISS_ROAD_FADE_STEP 8

struct ISSRoadSys {
    BOOL enabled;
    int volume;
    PlayerState *player;
    // The player's position on the last update
    VecFx32 lastPos;
};

static void ISSRoadSys_EnableCore(ISSRoadSys *sys);
static void func_0202b224(ISSRoadSys *sys);
static void ISSRoadSys_UpdateCore(ISSRoadSys *sys);
static BOOL ISSRoadSys_SyncPlayerPosIsChange(ISSRoadSys *sys);
static void ISSRoadSys_RaiseVolume(ISSRoadSys *sys);
static void ISSRoadSys_LowerVolume(ISSRoadSys *sys);
static void ISSRoadSys_CommitVolume(ISSRoadSys *sys);

ISSRoadSys *ISSRoadSys_Create(PlayerState *player, HeapID heapId) {
    ISSRoadSys *sys = GFL_HeapAllocate(heapId, sizeof(ISSRoadSys), FALSE, "iss_road_sys.c", 74);

    sys->enabled = FALSE;
    sys->volume = 0;
    sys->player = player;
    sys->lastPos = *PlayerState_GetWPos(player);
    return sys;
}

// pokebw2's ISSRoadSys_Free
void func_0202b1ec(ISSRoadSys *sys) {
    GFL_HeapFree(sys);
}

void ISSRoadSys_Update(ISSRoadSys *sys) {
    ISSRoadSys_UpdateCore(sys);
}

void ISSRoadSys_Enable(ISSRoadSys *sys) {
    ISSRoadSys_EnableCore(sys);
}

// pokebw2's ISSRoadSys_Disable
void func_0202b204(ISSRoadSys *sys) {
    func_0202b224(sys);
}

static void ISSRoadSys_EnableCore(ISSRoadSys *sys) {
    if (sys->enabled == FALSE) {
        sys->enabled = TRUE;
        sys->volume = 0;
        ISSRoadSys_CommitVolume(sys);
    }
}

// pokebw2's ISSRoadSys_DisableCore
static void func_0202b224(ISSRoadSys *sys) {
    if (sys->enabled) {
        sys->enabled = FALSE;
    }
}

static void ISSRoadSys_UpdateCore(ISSRoadSys *sys) {
    if (sys->enabled == FALSE) {
        return;
    }
    if (ISSRoadSys_SyncPlayerPosIsChange(sys)) {
        ISSRoadSys_RaiseVolume(sys);
    } else {
        ISSRoadSys_LowerVolume(sys);
    }
}

// Whether the player moved since the last update
static BOOL ISSRoadSys_SyncPlayerPosIsChange(ISSRoadSys *sys) {
    VecFx32 *pos = PlayerState_GetWPos(sys->player);
    BOOL changed;

    if (pos->x == sys->lastPos.x && pos->y == sys->lastPos.y && pos->z == sys->lastPos.z) {
        changed = FALSE;
    } else {
        changed = TRUE;
    }
    sys->lastPos = *pos;
    return changed;
}

static void ISSRoadSys_RaiseVolume(ISSRoadSys *sys) {
    if (sys->volume < SND_VOLUME_MAX) {
        int volume = sys->volume + ISS_ROAD_FADE_STEP;
        if (volume > SND_VOLUME_MAX) {
            volume = SND_VOLUME_MAX;
        }
        sys->volume = volume;
        ISSRoadSys_CommitVolume(sys);
    }
}

static void ISSRoadSys_LowerVolume(ISSRoadSys *sys) {
    if (sys->volume > 0) {
        int volume = sys->volume - ISS_ROAD_FADE_STEP;
        if (volume < 0) {
            volume = 0;
        }
        sys->volume = volume;
        ISSRoadSys_CommitVolume(sys);
    }
}

static void ISSRoadSys_CommitVolume(ISSRoadSys *sys) {
    GFL_SndBGMSetVolume(ISS_ROAD_TRACK_MASK, sys->volume);
}
