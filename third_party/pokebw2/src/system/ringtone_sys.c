#include "types.h"
#include "constants/sound.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/ui.h"
#include "nnsys/snd.h"
#include "system/player_volume_fader.h"
#include "system/ringtone_sys.h"

// The field sound system's ringtone. RingtoneSys_Create is swan's (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0); the other names are ours

// The sound player the ringtone plays on
#define RINGTONE_PLAYER 4
// How long the ringtone rings, in frames
#define RINGTONE_DURATION 900

enum {
    RINGTONE_STATE_IDLE,
    RINGTONE_STATE_RINGING,
    RINGTONE_STATE_LID_CLOSED,
    RINGTONE_STATE_RINGING_LID_CLOSED,
    RINGTONE_STATE_COUNT,
};

enum {
    RINGTONE_EVENT_STOP,
    RINGTONE_EVENT_RING,
    RINGTONE_EVENT_LID_OPEN,
    RINGTONE_EVENT_LID_CLOSE,
    RINGTONE_EVENT_COUNT,
};

struct RingtoneSys {
    u32 state;
    // Frames left to ring
    u16 timer;
    PlayerVolumeFader *fader;
};

static BOOL RingtoneSys_UpdateTimer(RingtoneSys *sys);
static void RingtoneSys_OnLidClose(void *work);
static void RingtoneSys_OnLidOpen(void *work);
static void RingtoneSys_StartTone(RingtoneSys *sys);
static void RingtoneSys_StopTone(RingtoneSys *sys);
static void RingtoneSys_SetMuted(RingtoneSys *sys, u8 muted);
static void RingtoneSys_SendEvent(RingtoneSys *sys, u8 event);
static void RingtoneSys_UpdateState(RingtoneSys *sys);

// The state each state goes to on each event
static const u8 sRingtoneStateTransitions[RINGTONE_STATE_COUNT][RINGTONE_EVENT_COUNT] = {
    [RINGTONE_STATE_IDLE] = { RINGTONE_STATE_IDLE, RINGTONE_STATE_RINGING, RINGTONE_STATE_IDLE,
                              RINGTONE_STATE_LID_CLOSED },
    [RINGTONE_STATE_RINGING] = { RINGTONE_STATE_IDLE, RINGTONE_STATE_RINGING, RINGTONE_STATE_RINGING,
                                 RINGTONE_STATE_RINGING_LID_CLOSED },
    [RINGTONE_STATE_LID_CLOSED] = { RINGTONE_STATE_LID_CLOSED, RINGTONE_STATE_RINGING_LID_CLOSED, RINGTONE_STATE_IDLE,
                                    RINGTONE_STATE_LID_CLOSED },
    [RINGTONE_STATE_RINGING_LID_CLOSED] = { RINGTONE_STATE_LID_CLOSED, RINGTONE_STATE_RINGING_LID_CLOSED,
                                            RINGTONE_STATE_RINGING, RINGTONE_STATE_RINGING_LID_CLOSED },
};

static RingtoneSys *sRingtoneSys;

RingtoneSys *RingtoneSys_Create(HeapID heapId, PlayerVolumeFader *fader) {
    RingtoneSys *sys = GFL_HeapAllocate(heapId, sizeof(RingtoneSys), TRUE, "ringtone_sys.c", 114);

    sys->timer = 0;
    sys->fader = fader;
    sRingtoneSys = sys;
    RingtoneSys_RestoreLidCallbacks();
    return sys;
}

void RingtoneSys_Free(RingtoneSys *sys) {
    GFL_HeapFree(sys);
}

void RingtoneSys_Update(RingtoneSys *sys) {
    RingtoneSys_UpdateState(sys);
}

void RingtoneSys_Ring(RingtoneSys *sys) {
    RingtoneSys_SendEvent(sys, RINGTONE_EVENT_RING);
}

void RingtoneSys_Stop(RingtoneSys *sys) {
    RingtoneSys_SendEvent(sys, RINGTONE_EVENT_STOP);
}

void RingtoneSys_RestoreLidCallbacks(void) {
    if (sRingtoneSys != NULL) {
        GCTX_HIDSetLidCallbacks(RingtoneSys_OnLidClose, RingtoneSys_OnLidOpen, sRingtoneSys);
    }
}

// Counts the ringtone down and keeps it playing; FALSE once it has rung its time
static BOOL RingtoneSys_UpdateTimer(RingtoneSys *sys) {
    if (sys->timer != 0) {
        sys->timer--;
    }
    if (sys->timer == 0) {
        if (GFL_SndPlayerIsActive(RINGTONE_PLAYER) == TRUE) {
            GFL_SndPlayerStop(RINGTONE_PLAYER);
        }
        return FALSE;
    }
    if (GFL_SndPlayerIsActive(RINGTONE_PLAYER) == FALSE) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_35, RINGTONE_PLAYER);
    }
    return TRUE;
}

static void RingtoneSys_OnLidClose(void *work) {
    RingtoneSys_SendEvent(sRingtoneSys, RINGTONE_EVENT_LID_CLOSE);
}

static void RingtoneSys_OnLidOpen(void *work) {
    RingtoneSys_SendEvent(sRingtoneSys, RINGTONE_EVENT_LID_OPEN);
}

static void RingtoneSys_StartTone(RingtoneSys *sys) {
    if (GFL_SndPlayerIsActive(RINGTONE_PLAYER) == FALSE) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_35, RINGTONE_PLAYER);
    }
    sys->timer = RINGTONE_DURATION;
}

static void RingtoneSys_StopTone(RingtoneSys *sys) {
    GFL_SndPlayerStop(RINGTONE_PLAYER);
    sys->timer = 0;
}

// Silences sound players 1 to 4 and the field's BGM fader, or gives them their volume back
static void RingtoneSys_SetMuted(RingtoneSys *sys, u8 muted) {
    if (muted == TRUE) {
        NNS_SndPlayerSetPlayerVolume(1, 0);
        NNS_SndPlayerSetPlayerVolume(2, 0);
        NNS_SndPlayerSetPlayerVolume(3, 0);
        NNS_SndPlayerSetPlayerVolume(4, 0);
    } else {
        NNS_SndPlayerSetPlayerVolume(1, SND_VOLUME_MAX);
        NNS_SndPlayerSetPlayerVolume(2, SND_VOLUME_MAX);
        NNS_SndPlayerSetPlayerVolume(3, SND_VOLUME_MAX);
        NNS_SndPlayerSetPlayerVolume(4, SND_VOLUME_MAX);
    }
    PlayerVolumeFader_SetMuted(sys->fader, muted);
}

static void RingtoneSys_SendEvent(RingtoneSys *sys, u8 event) {
    u8 state = sRingtoneStateTransitions[sys->state][event];

    switch (state) {
    case RINGTONE_STATE_IDLE:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        RingtoneSys_SetMuted(sys, FALSE);
        RingtoneSys_StopTone(sys);
        break;
    case RINGTONE_STATE_RINGING:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        RingtoneSys_SetMuted(sys, TRUE);
        RingtoneSys_StartTone(sys);
        break;
    case RINGTONE_STATE_LID_CLOSED:
        NNS_SndSetMasterVolume(0);
        RingtoneSys_StopTone(sys);
        break;
    case RINGTONE_STATE_RINGING_LID_CLOSED:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        RingtoneSys_SetMuted(sys, TRUE);
        RingtoneSys_StartTone(sys);
        break;
    }
    sys->state = state;
}

static void RingtoneSys_UpdateState(RingtoneSys *sys) {
    BOOL ringing = RingtoneSys_UpdateTimer(sys);

    switch (sys->state) {
    case RINGTONE_STATE_IDLE:
        break;
    case RINGTONE_STATE_RINGING:
        if (!ringing) {
            RingtoneSys_SendEvent(sys, RINGTONE_EVENT_STOP);
        }
        break;
    case RINGTONE_STATE_LID_CLOSED:
        break;
    case RINGTONE_STATE_RINGING_LID_CLOSED:
        if (!ringing) {
            RingtoneSys_SendEvent(sys, RINGTONE_EVENT_STOP);
        }
        break;
    }
}
