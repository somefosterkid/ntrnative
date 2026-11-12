// The sound archive's stream player for Black 2 and White 2 (their title music: pokebw2's src/system/snd_strm.c, the
// only caller), the port's own in place of libntrsystem's (docs/acore.md, "The archive stream"; the maintainers' notes, "The
// title's music"). port/include_b2w2/port_b2w2.h renames pokebw2's NNS_SndArcStrm* / NNS_SndStrmHandle* calls to the
// PORT_ functions here, so Black's and White's link and libntrsystem are untouched (they never play a stream).
//
// The stream is found with the archive's public calls (NNS_SndArcGetStrmInfo: file and volume; the stream player's
// channels; NNS_SndArcReadFile), read whole into host memory and decoded by acore (ac_strm.c), which plays it beside the
// hardware channels, on the music bus when it is split. Nothing goes in the sound heap, so the game's
// NNS_SndHeapSaveState / LoadState around the title see nothing of it. The samples are freed when the title frees its
// channels. Sound=libntr: no acore, the stream is silent and its position 0 (the game then sees it stopped).
//
// What the game relies on (snd_strm.c, ov162/title.c): Start plays from the offset (ms); Stop fades over its frames (game
// frames, here 560190 ARM7 cycles each of the sound's own time) and stops, 0 at once; the playing position (ms) is 0 once
// it stopped, which the title waits for after its fade before it frees the stream; a handle's player is NULL when it
// holds no stream.
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>

// NitroSystem's archive calls (NNS_SndArcGetStrmInfo ...) come with nitro.h (the port's prefix includes libntrsystem's
// headers). A stream handle as pokebw2's lib/nnsys/include/nnsys/snd.h declares it
typedef struct {
    void *player;
} StrmHandle;

// port/bridge/acore_bridge.c
extern void ab_lock(void);
extern void ab_unlock(void);
extern int ab_strm_load(const u8 *file, size_t size);
extern void ab_strm_free(void);
extern int ab_strm_set(int volume, u32 offsetMs);
extern void ab_strm_start(void);
extern void ab_strm_stop(int fadeFrames);
extern u32 ab_strm_pos(void);
extern int ab_strm_playing(void);

#define NO_FILE 0xFFFFFFFFu

static u32 sLoaded = NO_FILE;  // the archive file the bridge holds decoded
static u32 sLocked[2];         // the stream players' locked channels (the archive has one player)
static int sPlayer;            // a handle playing the stream points here

void PORT_SndArcStrmInit(u32 threadPrio, void *heap) {
    (void)threadPrio;
    (void)heap;
}

// the player's channels taken from the sequences while the title runs, as on the DS
BOOL PORT_SndArcStrmAllocChannel(int playerNo) {
    const NNSSndArcStrmPlayerInfo *info = NNS_SndArcGetStrmPlayerInfo(playerNo);
    if (info == NULL || playerNo < 0 || playerNo >= 2) {
        return FALSE;
    }
    u32 mask = 0;
    const u8 *ch = info->chNoList;  // (16 entries in the archive; libntrsystem's header declares 2)
    for (int i = 0; i < info->numChannels && i < 16; i++) {
        if (ch[i] < 16) {
            mask |= 1u << ch[i];
        }
    }
    SND_LockChannel(mask, 0);
    sLocked[playerNo] = mask;
    return TRUE;
}

void PORT_SndArcStrmFreeChannel(int playerNo) {
    if (playerNo >= 0 && playerNo < 2 && sLocked[playerNo] != 0) {
        SND_UnlockChannel(sLocked[playerNo], 0);
        sLocked[playerNo] = 0;
    }
    ab_strm_free();
    sLoaded = NO_FILE;
}

void PORT_SndStrmHandleInit(StrmHandle *handle) {
    handle->player = NULL;
}

void PORT_SndStrmHandleRelease(StrmHandle *handle) {
    handle->player = NULL;
}

BOOL PORT_SndArcStrmPrepare(StrmHandle *handle, int strmNo, u32 offset) {
    const NNSSndArcStrmInfo *info = NNS_SndArcGetStrmInfo(strmNo);
    ab_lock();
    ab_strm_stop(0);
    ab_unlock();
    handle->player = NULL;
    if (info == NULL) {
        return FALSE;
    }
    if (sLoaded != info->fileId) {
        u32 size = NNS_SndArcGetFileSize(info->fileId);
        u8 *file = size != 0 ? malloc(size) : NULL;
        if (file == NULL) {
            return FALSE;
        }
        int ok = NNS_SndArcReadFile(info->fileId, file, (s32)size, 0) == (s32)size && ab_strm_load(file, size);
        free(file);
        if (!ok) {
            return FALSE;
        }
        sLoaded = info->fileId;
        fprintf(stderr, "strm: stream %d (archive file %u, %u bytes) loaded, volume %d\n", strmNo, info->fileId, size,
                info->volume);
    }
    ab_lock();
    int ok = ab_strm_set(info->volume, offset);
    ab_unlock();
    if (!ok) {
        return FALSE;
    }
    handle->player = &sPlayer;
    return TRUE;
}

void PORT_SndArcStrmStartPrepared(StrmHandle *handle) {
    if (handle->player != &sPlayer) {
        return;
    }
    ab_lock();
    ab_strm_start();
    ab_unlock();
}

BOOL PORT_SndArcStrmStart(StrmHandle *handle, int strmNo, u32 offset) {
    if (!PORT_SndArcStrmPrepare(handle, strmNo, offset)) {
        return FALSE;
    }
    PORT_SndArcStrmStartPrepared(handle);
    return TRUE;
}

void PORT_SndArcStrmStop(StrmHandle *handle, int fadeFrames) {
    if (handle->player != &sPlayer) {
        return;
    }
    ab_lock();
    ab_strm_stop(fadeFrames);
    ab_unlock();
    if (fadeFrames <= 0) {
        handle->player = NULL;
    }
}

// in milliseconds; 0 (and the handle let go) once the stream has stopped
u32 PORT_SndArcStrmGetCurrentPlayingPos(StrmHandle *handle) {
    if (handle->player != &sPlayer) {
        return 0;
    }
    ab_lock();
    u32 pos = ab_strm_pos();
    int playing = ab_strm_playing();
    ab_unlock();
    if (!playing) {
        handle->player = NULL;
    }
    return pos;
}
