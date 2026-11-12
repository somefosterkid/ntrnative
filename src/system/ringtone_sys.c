// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/ringtone_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
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

static BOOL func_02029f8c(RingtoneSys *sys);
static void func_02029fd0(void *work);
static void func_02029fe4(void *work);
static void func_02029ff8(RingtoneSys *sys);
static void func_0202a01c(RingtoneSys *sys);
static void func_0202a02c(RingtoneSys *sys, u8 muted);
static void func_0202a084(RingtoneSys *sys, u8 event);
static void func_0202a0e8(RingtoneSys *sys);

// The state each state goes to on each event
// pokebw2's sRingtoneStateTransitions
static const u8 data_0209fed4[RINGTONE_STATE_COUNT][RINGTONE_EVENT_COUNT] = {
    [RINGTONE_STATE_IDLE] = { RINGTONE_STATE_IDLE, RINGTONE_STATE_RINGING, RINGTONE_STATE_IDLE,
                              RINGTONE_STATE_LID_CLOSED },
    [RINGTONE_STATE_RINGING] = { RINGTONE_STATE_IDLE, RINGTONE_STATE_RINGING, RINGTONE_STATE_RINGING,
                                 RINGTONE_STATE_RINGING_LID_CLOSED },
    [RINGTONE_STATE_LID_CLOSED] = { RINGTONE_STATE_LID_CLOSED, RINGTONE_STATE_RINGING_LID_CLOSED, RINGTONE_STATE_IDLE,
                                    RINGTONE_STATE_LID_CLOSED },
    [RINGTONE_STATE_RINGING_LID_CLOSED] = { RINGTONE_STATE_LID_CLOSED, RINGTONE_STATE_RINGING_LID_CLOSED,
                                            RINGTONE_STATE_RINGING, RINGTONE_STATE_RINGING_LID_CLOSED },
};

// pokebw2's sRingtoneSys
static RingtoneSys *data_02146944;

RingtoneSys *RingtoneSys_Create(HeapID heapId, PlayerVolumeFader *fader) {
    RingtoneSys *sys = GFL_HeapAllocate(heapId, sizeof(RingtoneSys), TRUE, "ringtone_sys.c", 114);

    sys->timer = 0;
    sys->fader = fader;
    data_02146944 = sys;
    func_02029f6c();
    return sys;
}

// pokebw2's RingtoneSys_Free
void func_02029f44(RingtoneSys *sys) {
    GFL_HeapFree(sys);
}

// pokebw2's RingtoneSys_Update
void func_02029f4c(RingtoneSys *sys) {
    func_0202a0e8(sys);
}

// pokebw2's RingtoneSys_Ring
void func_02029f54(RingtoneSys *sys) {
    func_0202a084(sys, RINGTONE_EVENT_RING);
}

// pokebw2's RingtoneSys_Stop
void func_02029f60(RingtoneSys *sys) {
    func_0202a084(sys, RINGTONE_EVENT_STOP);
}

// pokebw2's RingtoneSys_RestoreLidCallbacks
void func_02029f6c(void) {
    if (data_02146944 != NULL) {
        func_0203508c(func_02029fd0, func_02029fe4, data_02146944);
    }
}

// Counts the ringtone down and keeps it playing; FALSE once it has rung its time
// pokebw2's RingtoneSys_UpdateTimer
static BOOL func_02029f8c(RingtoneSys *sys) {
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

// pokebw2's RingtoneSys_OnLidClose
static void func_02029fd0(void *work) {
    func_0202a084(data_02146944, RINGTONE_EVENT_LID_CLOSE);
}

// pokebw2's RingtoneSys_OnLidOpen
static void func_02029fe4(void *work) {
    func_0202a084(data_02146944, RINGTONE_EVENT_LID_OPEN);
}

// pokebw2's RingtoneSys_StartTone
static void func_02029ff8(RingtoneSys *sys) {
    if (GFL_SndPlayerIsActive(RINGTONE_PLAYER) == FALSE) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_35, RINGTONE_PLAYER);
    }
    sys->timer = RINGTONE_DURATION;
}

// pokebw2's RingtoneSys_StopTone
static void func_0202a01c(RingtoneSys *sys) {
    GFL_SndPlayerStop(RINGTONE_PLAYER);
    sys->timer = 0;
}

// Silences sound players 1 to 4 and the field's BGM fader, or gives them their volume back
// pokebw2's RingtoneSys_SetMuted
static void func_0202a02c(RingtoneSys *sys, u8 muted) {
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
    func_020287a8(sys->fader, muted);
}

// pokebw2's RingtoneSys_SendEvent
static void func_0202a084(RingtoneSys *sys, u8 event) {
    u8 state = data_0209fed4[sys->state][event];

    switch (state) {
    case RINGTONE_STATE_IDLE:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        func_0202a02c(sys, FALSE);
        func_0202a01c(sys);
        break;
    case RINGTONE_STATE_RINGING:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        func_0202a02c(sys, TRUE);
        func_02029ff8(sys);
        break;
    case RINGTONE_STATE_LID_CLOSED:
        NNS_SndSetMasterVolume(0);
        func_0202a01c(sys);
        break;
    case RINGTONE_STATE_RINGING_LID_CLOSED:
        NNS_SndSetMasterVolume(SND_VOLUME_MAX);
        func_0202a02c(sys, TRUE);
        func_02029ff8(sys);
        break;
    }
    sys->state = state;
}

// pokebw2's RingtoneSys_UpdateState
static void func_0202a0e8(RingtoneSys *sys) {
    BOOL ringing = func_02029f8c(sys);

    switch (sys->state) {
    case RINGTONE_STATE_IDLE:
        break;
    case RINGTONE_STATE_RINGING:
        if (!ringing) {
            func_0202a084(sys, RINGTONE_EVENT_STOP);
        }
        break;
    case RINGTONE_STATE_LID_CLOSED:
        break;
    case RINGTONE_STATE_RINGING_LID_CLOSED:
        if (!ringing) {
            func_0202a084(sys, RINGTONE_EVENT_LID_OPEN);
        }
        break;
    }
}
