#include "types.h"
#include "gfl/heap.h"
#include "nnsys/snd.h"
#include "system/player_volume_fader.h"

// Fades a sound player's volume to a target over a number of frames. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except PlayerVolumeFader_SetMuted

#define VOLUME_MAX 127

struct PlayerVolumeFader {
    u8 player;
    u8 startVolume;
    u8 targetVolume;
    u16 duration;
    u16 frame;
    u8 muted;
};

static void PlayerVolumeFader_Init(PlayerVolumeFader *fader);
static void PlayerVolumeFader_SetTargetVolume(PlayerVolumeFader *fader, u8 volume, u16 duration);
static void PlayerVolumeFader_SetVolume(PlayerVolumeFader *fader, u8 volume);
static void PlayerVolumeFader_StepFrame(PlayerVolumeFader *fader);
static int PlayerVolumeFader_CalcVolume(PlayerVolumeFader *fader);
static void PlayerVolumeFader_Commit(PlayerVolumeFader *fader);

PlayerVolumeFader *PlayerVolumeFader_Create(HeapID heapId, u8 player) {
    PlayerVolumeFader *fader = GFL_HeapAllocate(heapId, sizeof(PlayerVolumeFader), FALSE, "player_volume_fader.c", 66);

    PlayerVolumeFader_Init(fader);
    fader->player = player;
    return fader;
}

void PlayerVolumeFader_Free(PlayerVolumeFader *fader) {
    GFL_HeapFree(fader);
}

void PlayerVolumeFader_Update(PlayerVolumeFader *fader) {
    PlayerVolumeFader_StepFrame(fader);
}

void PlayerVolumeFader_SetFade(PlayerVolumeFader *fader, u8 volume, u16 duration) {
    if (duration == 0) {
        PlayerVolumeFader_SetVolume(fader, volume);
    } else {
        PlayerVolumeFader_SetTargetVolume(fader, volume, duration);
    }
}

void PlayerVolumeFader_SetMuted(PlayerVolumeFader *fader, u8 muted) {
    fader->muted = muted;
    PlayerVolumeFader_Commit(fader);
}

static void PlayerVolumeFader_Init(PlayerVolumeFader *fader) {
    fader->player = 0;
    fader->startVolume = VOLUME_MAX;
    fader->targetVolume = VOLUME_MAX;
    fader->duration = 0;
    fader->frame = 0;
    fader->muted = FALSE;
    PlayerVolumeFader_Commit(fader);
}

static void PlayerVolumeFader_SetTargetVolume(PlayerVolumeFader *fader, u8 volume, u16 duration) {
    fader->startVolume = PlayerVolumeFader_CalcVolume(fader);
    fader->targetVolume = volume;
    fader->duration = duration;
    fader->frame = 0;
}

static void PlayerVolumeFader_SetVolume(PlayerVolumeFader *fader, u8 volume) {
    fader->startVolume = volume;
    fader->targetVolume = volume;
    fader->duration = 0;
    fader->frame = 0;
    PlayerVolumeFader_Commit(fader);
}

static void PlayerVolumeFader_StepFrame(PlayerVolumeFader *fader) {
    if (fader->frame < fader->duration) {
        fader->frame++;
        PlayerVolumeFader_Commit(fader);
    }
}

static int PlayerVolumeFader_CalcVolume(PlayerVolumeFader *fader) {
    int volume = fader->startVolume;
    u8 target = fader->targetVolume;
    u16 duration = fader->duration;
    u16 frame = fader->frame;

    if (frame != 0) {
        volume += (target - volume) * frame / duration;
    }
    return volume;
}

static void PlayerVolumeFader_Commit(PlayerVolumeFader *fader) {
    int volume = PlayerVolumeFader_CalcVolume(fader);

    if (fader->muted) {
        volume = 0;
    }
    NNS_SndPlayerSetPlayerVolume(fader->player, volume);
}
