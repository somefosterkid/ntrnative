// port/acore behind the game's sound path (docs/acore.md piece 18, the game's side). With sim_config.ini's Sound=acore
// (BWPORT_SOUND=acore|libntr overrides the file; libntr-acore.patch) the game's sound plays through port/acore, the
// clean-room DS sound core, instead of libntr's sim7 driver and mixer:
//   - commands: port/src/acapture.c's hook (the capture's translation, the host's structures back to the DS's layout)
//     sees every command list the ARM9 sends; with the live path on it calls acglue_mem / acglue_cmds / acglue_wake
//     here, which write into acore's 4 MB memory and queue the list (port/bridge/acore_bridge.c, the acore side);
//     libntr's PXI thread then skips sim7 (SIM_soundBackendOn);
//   - samples: SDL's audio callback calls Mix(), which runs acore for the callback's frames at the DS rate
//     (32728.5 Hz) and resamples to the device's rate; no callback, no sound and no progress (libntr-audiofallback.patch
//     keeps SDL's dummy driver running when there is no device);
//   - what the game reads back: the shared work (finishCommandTag, player and channel status, ticks, variables) is a
//     copy of acore's after each callback chunk; READ_DRIVER_INFO's buffer, in the DS layout in acore's memory, is
//     translated into the game's SNDDriverInfo (host structs: bit fields, 64-bit pointers) when the list holding the
//     command has finished.
#include <nitro.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libntr (libntr-acore.patch, include/simulator/sim_audio.h)
typedef struct SIM_SoundBackend {
    int (*start)(int aAudioFrequency);
    void (*mix)(s16 *aStereo, int aFrames, int aMasterVolume);
    int (*run)(uint64_t aToCycle, s16 *aStereo, int aMaxFrames);
    void (*busSplit)(int aOn);                  // libntr-mu83.patch: the music bus
    int (*busRun)(s16 *aStereo, int aFrames);
    void (*busSlot)(int aSlot, int aMusic);
    void (*setQuality)(int aEnhanced);          // libntr-ios-soundq.patch
    int (*waitStuck)(char *aState, int aSize);  // libntr-ios-audiowait.patch
} SIM_SoundBackend;
extern const SIM_SoundBackend *SIM_soundBackend;

extern void SDL_Delay(uint32_t ms);

// port/src/acapture.c
extern void acapture_hook_install(void);

// port/bridge/acore_bridge.c (acore's own types: plain integers here)
extern void ab_lock(void);
extern void ab_unlock(void);
extern int ab_init(unsigned out_rate);
extern void ab_reset(void);
extern uint8_t *ab_mem(void);
extern void ab_mem_write(uint32_t ds, const uint8_t *buf, uint32_t len);
extern int ab_queue(const uint32_t (*cmd)[5], uint32_t n);
extern void ab_wake(void);
extern void ab_render(int16_t *out, int frames, unsigned vol);
extern int ab_run(uint64_t to, int16_t *out, int max);
extern uint64_t ab_now(void);
extern void ab_split(int on);
extern int ab_run_music(int16_t *out, int frames);
extern void ab_slot_music(int slot, int music);
extern void ab_set_quality(int enhanced);
extern uint32_t ab_nqueue(void);
extern uint32_t ab_finish_tag(void);
extern void ab_finish_bump(void);

#define DS_BASE 0x02000000u
#define DS_MASK 0x003FFFFFu
#define DS_INFO 0x0237D000u
#define DS_SHARED 0x0237F000u
#define MAX_INFO 16

_Static_assert(sizeof(SNDSharedWork) == 0x280, "SNDSharedWork is the DS's 0x280 bytes");
_Static_assert(offsetof(SNDSharedWork, player) == 0x20 && offsetof(SNDSharedWork, globalVariable) == 0x260,
               "SNDSharedWork layout");

static int sLive;
static SNDSharedWork *sShared;       // the game's, from the SHARED_WORK command
static uint32_t sLists;              // command lists queued so far (acore's finishCommandTag counts the same)
static uint32_t sDropped;            // lists acglue_cmds could not queue
static struct {
    SNDDriverInfo *dest;
    uint32_t tag; // done when acore's finishCommandTag reaches it
} sInfo[MAX_INFO];
static int sNInfo;
static int sSplit = -1, sQuality = -1;   // libntr's last BusSplit and SetQuality, for a new core after a reset

int acglue_live(void) {
    return sLive;
}

// the live core's clock (the cycle its last frame step ended at): the capture stamps a list there, which is where the
// core takes it, so the oracle replays the core's own timeline
uint64_t acglue_now(void) {
    ab_lock();
    uint64_t t = ab_now();
    ab_unlock();
    return t;
}

void acglue_mem(u32 ds, const u8 *buf, u32 len) {
    ab_lock();
    ab_mem_write(ds, buf, len);
    ab_unlock();
}

void acglue_cmds(const u32 (*cmd)[5], u32 n) {
    // the audio callback drains the queue (every ~5 ms); wait for room, but not forever
    for (int tries = 0;; tries++) {
        ab_lock();
        int ok = ab_queue(cmd, n);
        if (ok) sLists++;
        ab_unlock();
        if (ok) return;
        if (tries > 500) {
            fprintf(stderr, "acore: the command queue stayed full, a list is dropped\n");
            ab_lock();
            sDropped++;
            ab_unlock();
            return;
        }
        SDL_Delay(1);
    }
}

void acglue_wake(void) {
    ab_lock();
    ab_wake();
    ab_unlock();
}

// SHARED_WORK: the game's struct (acore's copy of it is written back after each chunk)
void acglue_shared(void *host) {
    ab_lock();
    sShared = (SNDSharedWork *)host;
    ab_unlock();
}

// READ_DRIVER_INFO: the game's buffer; filled once acore has run the list this command is in (the next list queued
// is number sLists + 1)
void acglue_info(void *host) {
    ab_lock();
    if (sNInfo < MAX_INFO) {
        sInfo[sNInfo].dest = (SNDDriverInfo *)host;
        sInfo[sNInfo].tag = sLists + 1;
        sNInfo++;
    }
    ab_unlock();
}

// ---- acore's DS-layout memory read as little-endian fields ----
static inline u8 R8(const u8 *p) {
    return p[0];
}
static inline u16 R16(const u8 *p) {
    return (u16)(p[0] | p[1] << 8);
}
static inline u32 R32(const u8 *p) {
    return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24;
}

// a DS address in the work image (the work sits at the info's workAddr; its channels first) -> the game's channel
static SNDExChannel *Chan(SNDDriverInfo *h, u32 workAddr, u32 a) {
    if (!a) return NULL;
    u32 i = (a - workAddr) / 0x54;
    return i < SND_CHANNEL_NUM ? &h->work.channel[i] : NULL;
}

// the DS-layout info buffer (SNDWork 0x1180, 16 SOUNDxCNT, the work's address, the locked channels) into the host's
static void TranslateInfo(SNDDriverInfo *h, const u8 *d) {
    u32 workAddr = R32(d + 0x1180 + 64);
    for (int i = 0; i < SND_CHANNEL_NUM; i++) {
        const u8 *c = d + 0x54 * i;
        SNDExChannel *o = &h->work.channel[i];
        o->myNo = R8(c + 0x00);
        o->type = R8(c + 0x01);
        o->env_status = R8(c + 0x02);
        u8 f = R8(c + 0x03);
        o->active_flag = f & 1;
        o->start_flag = (f >> 1) & 1;
        o->auto_sweep = (f >> 2) & 1;
        o->sync_flag = f >> 3;
        o->pan_range = R8(c + 0x04);
        o->original_key = R8(c + 0x05);
        o->user_decay2 = (s16)R16(c + 0x06);
        o->key = R8(c + 0x08);
        o->velocity = R8(c + 0x09);
        o->init_pan = (s8)R8(c + 0x0A);
        o->user_pan = (s8)R8(c + 0x0B);
        o->user_decay = (s16)R16(c + 0x0C);
        o->user_pitch = (s16)R16(c + 0x0E);
        o->env_decay = (s32)R32(c + 0x10);
        o->sweep_counter = (s32)R32(c + 0x14);
        o->sweep_length = (s32)R32(c + 0x18);
        o->attack = R8(c + 0x1C);
        o->sustain = R8(c + 0x1D);
        o->decay = R16(c + 0x1E);
        o->release = R16(c + 0x20);
        o->prio = R8(c + 0x22);
        o->pan = R8(c + 0x23);
        o->volume = R16(c + 0x24);
        o->timer = R16(c + 0x26);
        o->sweep_pitch = (s16)R16(c + 0x32);
        o->length = (s32)R32(c + 0x34);
        o->nextLink = Chan(h, workAddr, R32(c + 0x50));
    }
    for (int i = 0; i < SND_PLAYER_NUM; i++) {
        const u8 *p = d + 0x540 + 0x24 * i;
        SNDPlayer *o = &h->work.player[i];
        u8 f = R8(p);
        o->active_flag = f & 1;
        o->prepared_flag = (f >> 1) & 1;
        o->pause_flag = (f >> 2) & 1;
        o->myNo = R8(p + 1);
        o->prio = R8(p + 4);
        o->volume = R8(p + 5);
        o->extFader = (s16)R16(p + 6);
        for (int t = 0; t < SND_TRACK_NUM_PER_PLAYER; t++) o->tracks[t] = R8(p + 8 + t);
        o->tempo = R16(p + 0x18);
        o->tempo_ratio = R16(p + 0x1A);
        o->tempo_counter = R16(p + 0x1C);
    }
    for (int i = 0; i < SND_TRACK_NUM; i++) {
        const u8 *t = d + 0x780 + 0x40 * i;
        SNDTrack *o = &h->work.track[i];
        u8 f = R8(t);
        o->active_flag = f & 1;
        o->note_wait = (f >> 1) & 1;
        o->mute_flag = (f >> 2) & 1;
        o->tie_flag = (f >> 3) & 1;
        o->note_finish_wait = (f >> 4) & 1;
        o->porta_flag = (f >> 5) & 1;
        o->cmp_flag = (f >> 6) & 1;
        o->channel_mask_flag = f >> 7;
        o->pan_range = R8(t + 1);
        o->prgNo = R16(t + 2);
        o->volume = R8(t + 4);
        o->volume2 = R8(t + 5);
        o->pitch_bend = (s8)R8(t + 6);
        o->bend_range = R8(t + 7);
        o->pan = (s8)R8(t + 8);
        o->ext_pan = (s8)R8(t + 9);
        o->extFader = (s16)R16(t + 0x0A);
        o->ext_pitch = (s16)R16(t + 0x0C);
        o->attack = R8(t + 0x0E);
        o->decay = R8(t + 0x0F);
        o->sustain = R8(t + 0x10);
        o->release = R8(t + 0x11);
        o->prio = R8(t + 0x12);
        o->transpose = (s8)R8(t + 0x13);
        o->porta_key = R8(t + 0x14);
        o->porta_time = R8(t + 0x15);
        o->sweep_pitch = (s16)R16(t + 0x16);
        o->channel_mask = R16(t + 0x1E);
        o->wait = (s32)R32(t + 0x20);
        o->call_stack_depth = R8(t + 0x3B);
        o->channel_list = Chan(h, workAddr, R32(t + 0x3C));
    }
    for (int i = 0; i < SND_CHANNEL_NUM; i++) h->chCtrl[i] = R32(d + 0x1180 + 4 * i);
    h->workAddress = &h->work;
    h->lockedChannels = R32(d + 0x1180 + 64 + 4);
}

// after each chunk (the lock held): the driver infos whose list has finished, then the shared work
static void CopyBack(void) {
    const u8 *mem = ab_mem();
    if (!sShared) return;
    u32 done = R32(mem + ((DS_SHARED - DS_BASE) & DS_MASK));
    int k = 0;
    for (int i = 0; i < sNInfo; i++) {
        if ((int32_t)(done - sInfo[i].tag) >= 0) {
            TranslateInfo(sInfo[i].dest, mem + ((DS_INFO - DS_BASE) & DS_MASK));
        } else {
            sInfo[k++] = sInfo[i];
        }
    }
    sNInfo = k;
    // the shared work last: finishCommandTag tells the game its info is there
    memcpy((void *)sShared, mem + ((DS_SHARED - DS_BASE) & DS_MASK), sizeof(SNDSharedWork));
}

static void Mix(s16 *out, int frames, int vol) {
    ab_lock();
    ab_render(out, frames, (unsigned)vol);
    CopyBack();
    ab_unlock();
}

// the frame clock (libntr-soundclock.patch): acore to the cycle a game frame ends at; DS-rate frames back
static int Run(uint64_t to, s16 *out, int max) {
    ab_lock();
    int n = ab_run(to, out, max);
    CopyBack();
    ab_unlock();
    return n;
}

static int Start(int rate) {
    if (!ab_init((unsigned)rate)) return 0;
    sLive = 1;
    acapture_hook_install();
    return 1;
}

// the music bus (libntr-mu83.patch, docs/acore.md "Game speed and sound")
static void BusSplit(int on) {
    ab_lock();
    ab_split(on);
    sSplit = on;
    ab_unlock();
}

static int BusRun(s16 *out, int frames) {
    ab_lock();
    int n = ab_run_music(out, frames);
    ab_unlock();
    return n;
}

static void BusSlot(int slot, int music) {
    ab_lock();
    ab_slot_music(slot, music);
    ab_unlock();
}

// SoundQuality (libntr-ios-soundq.patch): libntr calls it with the audio locked
static void SetQuality(int enhanced) {
    ab_lock();
    ab_set_quality(enhanced);
    sQuality = enhanced;
    ab_unlock();
}

// a command wait had no reply for 300 ms (libntr-ios-audiowait.patch; libntr calls it with the audio locked): the game waits
// for its finished tag to move, which acore moves once per list it runs. With no list left in acore's queue the reply cannot
// come (a list lost on its way): count one finished, so that the game goes on
static int WaitStuck(char *st, int n) {
    ab_lock();
    uint32_t q = ab_nqueue(), fin = ab_finish_tag();
    uint32_t game = sShared ? sShared->finishCommandTag : 0;
    int release = q == 0 && sShared != NULL;
    if (release) ab_finish_bump();
    snprintf(st, (size_t)n, "acore: %u lists queued in all, %u dropped, %u waiting, %u finished; the game's finished tag %u%s", sLists,
             sDropped, q, fin, game, sShared ? "" : ", no shared work");
    ab_unlock();
    return release;
}
// the in-process restart (machine.c, ab_lock held): a new core with libntr's settings, and nothing of the game's sound
// work known (the game sends its SHARED_WORK command again at boot)
void acglue_reset(void) {
    if (!sLive) return;
    ab_reset();
    if (sSplit >= 0) ab_split(sSplit);
    if (sQuality >= 0) ab_set_quality(sQuality);
    sShared = NULL;
    sLists = 0;
    sDropped = 0;
    sNInfo = 0;
}

static const SIM_SoundBackend sBackend = {Start, Mix, Run, BusSplit, BusRun, BusSlot, SetQuality, WaitStuck};

__attribute__((constructor)) static void AcoreGlueInit(void) {
    SIM_soundBackend = &sBackend;
}
