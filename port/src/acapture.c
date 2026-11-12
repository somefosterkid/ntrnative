// The sound capture for port/acore (docs/acore.md "The capture"): BWPORT_ACAPTURE=FILE.acs[,FRAMES|,FROM-TO] writes
// every sound command list the ARM9 sends to the ARM7 (before it runs) and every wakeup word, with the memory they
// point at, as a .acs stream (docs/acore.md "The stream: .acs"). Off unless the variable is set.
//
// The hook is libntr-acapture.patch's SIM_sndCaptureHook in sim7's PXI sound path. It runs in libntr's PXI thread;
// the port's sound structures are the host's (8-byte pointers, libntrsystem's PC copy of a bank), so each pointer
// is translated to the DS's address plan and the files put back into the DS file layout:
//   - the sound heap (g_GFLSndHeap) is mapped linearly at 0x02100000 (DS address = 0x02100000 + host offset);
//   - a bank: libntrsystem's copy keeps the file's 0x18-byte header, then 64 bytes of host links (4 x {waveArc, next}
//     host pointers), then the file's bytes from 0x38 on at +0x58. The DS file: header, 4 x {u32 waveArc, u32 next}
//     (0x20 bytes), the rest. The links become the DS addresses of the wave archives;
//   - a wave archive: the file (topLink cleared); a waveOffset entry >= 0x02000000 is a host pointer (the low 32
//     bits, SND_SetWaveDataAddress) to a SNDWaveData in the heap: that wave is put at its own DS address and the entry
//     becomes it. An archive loaded as a table only (no entry below 0x02000000) is cut after its table;
//   - wave-out samples (SETUP_CHANNEL_PCM) and capture buffers are not in the heap: a ring at 0x02230000-0x0237CFFF;
//   - the shared work and the driver info buffer: 0x0237F000 / 0x0237D000.
// The same translation feeds the live path (Sound=acore, port/src/acore_glue.c): the MEM chunks become writes into
// acore's memory and the CMDS lists queued commands, at once; there is no FROM / TO and no clock (acore's is the audio
// callback's). BWPORT_ACAPTURE may be set as well: the file records what the live core was given.
// Time: frame f is f * 560190 ARM7 cycles (OS_GetVBlankCount at the hook), events at one frame in arrival order.
// Frames before FROM: only the global state commands (shared work, master volume and pan, output selector), at time 0:
// a clip starts quiet, so FROM wants to be before the music it is meant to hold begins.
#include <nitro.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void (*SIM_sndCaptureHook)(const SNDCommand *list);
#include "port_game.h"
#define g_GFLSndHeap (PORT_Game->sndHeap) // src/system/snd_sys.c: the NNS sound heap's memory, the running game's

// SNDPlayer's and SNDTrack's fields before the first pointer sit at the DS's offsets (PLAYER_PARAM / TRACK_PARAM take
// offsets into them as they are)
_Static_assert(offsetof(SNDPlayer, extFader) == 6 && offsetof(SNDPlayer, tempo_ratio) == 0x1A, "SNDPlayer layout");
_Static_assert(offsetof(SNDTrack, mod) == 24, "SNDTrack layout");

#define HEAP_LEN (PORT_Game->sndHeapLen) // the running game's (Black's WORK_AREA_SIZE(0x95ad0), Black 2's 0x9d000)
#define DS_HEAP 0x02100000u
#define DS_RING_LO 0x02230000u
#define DS_RING_HI 0x0237D000u
#define DS_INFO 0x0237D000u
#define DS_SHARED 0x0237F000u
#define FRAME_CYCLES 560190ull
#define NREC 8192
#define NREG 128

typedef struct {
    u32 ds, len;
    u64 hash;
} Rec;
typedef struct {
    uintptr_t host;
    u32 ds, len;
} Ring;

static FILE *sOut;
static u32 sFrom, sTo = 0xFFFFFFFFu;
static u64 sNow, sLastNow;
static u32 sLastFrame;
static int sClosed;
static Rec sRec[NREC];
static int sNRec;
static Ring sRing[256];
static int sNRing;
static u32 sRingNext = DS_RING_LO;
static const void *sBanks[NREG];
static int sNBanks;
static u64 sNCmds, sNLists, sNWake, sNMem, sMemBytes;
static int sInRefresh;

// the live path (acore_glue.c)
extern int acglue_live(void);
extern u64 acglue_now(void);
extern void acglue_mem(u32 ds, const u8 *buf, u32 len);
extern void acglue_cmds(const u32 (*cmd)[5], u32 n);
extern void acglue_wake(void);
extern void acglue_shared(void *host);
extern void acglue_info(void *host);

static void W32(u8 *p, u32 v) {
    p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24);
}
static void W64(u8 *p, u64 v) {
    W32(p, (u32)v), W32(p + 4, (u32)(v >> 32));
}
static u32 R32(const u8 *p) {
    return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24;
}

static void Chunk(const char *tag, const void *a, u32 an, const void *b, u32 bn) {
    if (!sOut) return;
    u8 h[8];
    u32 sz = an + bn, pad = (4 - (sz & 3)) & 3;
    static const u8 zero[4];
    memcpy(h, tag, 4);
    W32(h + 4, sz);
    fwrite(h, 1, 8, sOut);
    if (an) fwrite(a, 1, an, sOut);
    if (bn) fwrite(b, 1, bn, sOut);
    if (pad) fwrite(zero, 1, pad, sOut);
}

// FNV-1a over 8-byte words (then the odd bytes): the live path hashes every loaded bank and wave archive again at each
// sequence start (200-250 KB in the town), byte by byte that was ~4 cycles a byte on the PXI thread
static u64 Hash(const u8 *p, u32 n) {
    u64 h = 1469598103934665603ull;
    u32 i = 0;
    for (; i + 8 <= n; i += 8) {
        u64 w;
        memcpy(&w, p + i, 8);
        h = (h ^ w) * 1099511628211ull;
    }
    for (; i < n; i++) h = (h ^ p[i]) * 1099511628211ull;
    return h;
}

// MEM chunk unless the DS range holds these bytes already
static void Mem(u32 ds, const u8 *buf, u32 len) {
    u64 h = Hash(buf, len);
    int i;
    for (i = 0; i < sNRec; i++) {
        if (sRec[i].ds == ds) break;
    }
    if (i < sNRec && sRec[i].len == len && sRec[i].hash == h) return;
    // drop other records this range overlaps
    for (int k = 0; k < sNRec; k++) {
        if (k != i && sRec[k].ds < ds + len && ds < sRec[k].ds + sRec[k].len) {
            sRec[k] = sRec[--sNRec];
            if (i == sNRec) i = k;
            k--;
        }
    }
    if (i >= sNRec) {
        if (sNRec == NREC) i = 0;
        else i = sNRec++;
    }
    sRec[i].ds = ds, sRec[i].len = len, sRec[i].hash = h;
    if (acglue_live()) acglue_mem(ds, buf, len);
    u8 head[12];
    W64(head, sNow), W32(head + 8, ds);
    Chunk("MEM ", head, 12, buf, len);
    sNMem++, sMemBytes += len;
}

static int InHeap(const void *p) {
    uintptr_t a = (uintptr_t)p, b = (uintptr_t)g_GFLSndHeap;
    return a >= b && a <= b + HEAP_LEN;
}

static u32 HeapDs(const void *p) {
    return DS_HEAP + (u32)((uintptr_t)p - ((uintptr_t)g_GFLSndHeap & ~(uintptr_t)31));
}

// a DS address for host memory of len bytes: the heap's, or a ring slot (kept per host address)
static u32 MapHost(const void *p, u32 len) {
    if (InHeap(p)) return HeapDs(p);
    for (int i = 0; i < sNRing; i++) {
        if (sRing[i].host == (uintptr_t)p && sRing[i].len >= len) return sRing[i].ds;
    }
    u32 need = (len + 31) & ~31u;
    if (sRingNext + need > DS_RING_HI) sRingNext = DS_RING_LO;
    u32 ds = sRingNext;
    sRingNext += need;
    // slots the new one covers are gone
    for (int i = 0; i < sNRing; i++) {
        if (sRing[i].ds < ds + need && ds < sRing[i].ds + sRing[i].len) sRing[i--] = sRing[--sNRing];
    }
    if (sNRing == 256) sNRing = 0;
    sRing[sNRing].host = (uintptr_t)p, sRing[sNRing].ds = ds, sRing[sNRing].len = need, sNRing++;
    return ds;
}

// a heap pointer from a u32 the game stored (SND_SetWaveDataAddress truncates the host pointer)
static const u8 *HeapFromLow32(u32 v) {
    uintptr_t base = (uintptr_t)g_GFLSndHeap;
    for (int k = 0; k < 2; k++) {
        uintptr_t a = ((base & ~(uintptr_t)0xFFFFFFFFu) + ((uintptr_t)k << 32)) | v;
        if (a >= base && a < base + HEAP_LEN) return (const u8 *)a;
    }
    return NULL;
}

static int Magic(const u8 *p, const char *m) {
    return p && !memcmp(p, m, 4);
}

// a wave's size from its SNDWaveParam {format, loop, rate u16, time u16, loopStart u16, loopLen u32}
static u32 WaveLen(const u8 *w) {
    u32 loopStart = w[8] | w[9] << 8, loopLen = R32(w + 10);
    u32 n = 12 + (loopStart + loopLen) * 4;
    return n > 0x100000 ? 0 : n;
}

// the wave archive at host p, as a DS file at its address; returns the address
static u32 EmitWaveArc(const u8 *p) {
    if (!Magic(p, "SWAR") || !InHeap(p)) return 0;
    u32 fsize = R32(p + 8), count = R32(p + 0x38);
    if (count > 0x1000 || fsize > 0x200000) return 0;
    int tableOnly = 0;
    for (u32 i = 0; i < count; i++) {
        if (R32(p + 0x3C + 4 * i) >= 0x02000000u) tableOnly = 1;
    }
    u32 len = tableOnly ? 0x3C + 4 * count : fsize;
    if (len < 0x3C + 4 * count) len = 0x3C + 4 * count;
    u8 *buf = malloc(len);
    memcpy(buf, p, len);
    memset(buf + 0x18, 0, 0x20); // topLink + reserved
    for (u32 i = 0; i < count; i++) {
        u32 v = R32(p + 0x3C + 4 * i);
        if (v < 0x02000000u) continue; // an offset in the file
        const u8 *w = HeapFromLow32(v);
        u32 wl = w ? WaveLen(w) : 0;
        if (!wl) {
            W32(buf + 0x3C + 4 * i, 0);
            continue;
        }
        u32 wds = HeapDs(w);
        Mem(wds, w, wl);
        W32(buf + 0x3C + 4 * i, wds);
    }
    u32 ds = HeapDs(p);
    Mem(ds, buf, len);
    free(buf);
    return ds;
}

static void AddBank(const void *p) {
    for (int i = 0; i < sNBanks; i++) {
        if (sBanks[i] == p) return;
    }
    if (sNBanks < NREG) sBanks[sNBanks++] = p;
}

// the bank in libntrsystem's PC layout, put back into the DS file layout; returns its DS address
static u32 EmitBank(const u8 *p) {
    if (!Magic(p, "SBNK") || !InHeap(p)) return 0;
    u32 len = R32(p + 8);
    if (len < 0x40 || len > 0x100000) return 0;
    u8 *buf = malloc(len);
    memcpy(buf, p, 0x18);
    memset(buf + 0x18, 0, 0x20);
    memcpy(buf + 0x38, p + 0x58, len - 0x38);
    for (int i = 0; i < 4; i++) {
        const u8 *wa = NULL;
        memcpy(&wa, p + 0x18 + 16 * i, sizeof wa);
        if (!wa) continue;
        u32 d = EmitWaveArc(wa);
        W32(buf + 0x18 + 8 * i, d);
    }
    u32 ds = HeapDs(p);
    Mem(ds, buf, len);
    free(buf);
    return ds;
}

static void Refresh(void) {
    for (int i = 0; i < sNBanks; i++) EmitBank(sBanks[i]);
}

// a sequence or sequence archive file, from the pointer the command carries (the file's start, or inside it)
static u32 EmitSeq(const u8 *p) {
    if (!p || !InHeap(p)) return p ? MapHost(p, 0x20) : 0;
    const u8 *f = p;
    for (u32 back = 0; back < 0x40000; back += 4, f -= 4) {
        if ((uintptr_t)f < (uintptr_t)g_GFLSndHeap) break;
        if ((Magic(f, "SSEQ") || Magic(f, "SSAR")) && R32(f + 8) > (u32)(p - f)) {
            u32 len = R32(f + 8);
            if (len > 0x100000) break;
            Mem(HeapDs(f), f, len);
            return HeapDs(p);
        }
    }
    return HeapDs(p);
}

static u32 Tr(u64 v) { // a pointer argument into the heap, else its low bits
    return InHeap((const void *)(uintptr_t)v) ? HeapDs((const void *)(uintptr_t)v) : (u32)v;
}

static void Info(void) {
    char t[256];
    int n = snprintf(t, sizeof t, "source=bwport acapture\nmode=driver\nframes=%u\nfrom=%u\ntick0=0\n",
                     sTo == 0xFFFFFFFFu ? 0 : sTo - sFrom, sFrom);
    Chunk("INFO", t, (u32)n, NULL, 0);
}

static void Close(void) {
    if (sClosed || !sOut) return;
    sClosed = 1;
    u8 e[8];
    u64 t = sTo != 0xFFFFFFFFu ? (u64)(sTo - sFrom) * FRAME_CYCLES : sLastNow + FRAME_CYCLES;
    W64(e, t);
    Chunk("END ", e, 8, NULL, 0);
    fclose(sOut);
    sOut = NULL;
    fprintf(stderr, "acapture: %llu lists, %llu commands, %llu wakes, %llu MEM (%llu bytes), to frame %u\n",
            (unsigned long long)sNLists, (unsigned long long)sNCmds, (unsigned long long)sNWake,
            (unsigned long long)sNMem, (unsigned long long)sMemBytes, sLastFrame);
}

void acapture_close(void) { Close(); }

// frames before FROM: the commands that set global state (no sound: a clip starts quiet)
static int Keep(const SNDCommand *c, u32 frame) {
    if (frame >= sFrom) return 1;
    switch (c->id) {
    case SND_COMMAND_SHARED_WORK:
    case SND_COMMAND_MASTER_VOLUME:
    case SND_COMMAND_MASTER_PAN:
    case SND_COMMAND_OUTPUT_SELECTOR:
    case SND_COMMAND_SURROUND_DECAY:
        return 1;
    default:
        return 0;
    }
}

static void Hook(const SNDCommand *list) {
    int live = acglue_live();
    if (sClosed && !live) return;
    u32 frame = OS_GetVBlankCount();
    if (!sClosed && frame > sTo) {
        Close();
        if (!live) return;
    }
    sLastFrame = frame;
    sNow = frame < sFrom ? 0 : (u64)(frame - sFrom) * FRAME_CYCLES;
    // live: the time the core takes the list at (its clock, not the PXI thread's frame, which is racy by a frame)
    if (live && frame >= sFrom) {
        u64 t = acglue_now(), f0 = (u64)sFrom * FRAME_CYCLES;
        sNow = t > f0 ? t - f0 : 0;
    }
    if (sNow < sLastNow) sNow = sLastNow;
    sLastNow = sNow;
    u8 tm[8];
    W64(tm, sNow);
    if (!list) {
        if (frame >= sFrom) Chunk("WAKE", tm, 8, NULL, 0), sNWake++;
        if (live) acglue_wake();
        return;
    }
    // the memory first (a MEM at the list's time comes before its CMDS). Before FROM only the global state is kept
    int n = 0, nall = 0;
    for (const SNDCommand *c = list; c; c = c->next) n += Keep(c, frame), nall++;
    if (!n && !live) return;
    u8 *cmds = malloc(16 + 20 * (size_t)n);
    u32(*lv)[5] = live ? malloc(sizeof(u32[5]) * (size_t)nall) : NULL;
    W64(cmds, sNow), W32(cmds + 8, frame), W32(cmds + 12, (u32)n);
    int k = 0, kl = 0;
    for (const SNDCommand *c = list; c; c = c->next) {
        int keep = Keep(c, frame);
        if (!keep && !live) continue;
        u32 a[4];
        for (int j = 0; j < 4; j++) a[j] = (u32)c->arg[j];
        switch (c->id) {
        case SND_COMMAND_START_SEQ:
        case SND_COMMAND_PREPARE_SEQ: {
            a[1] = EmitSeq((const u8 *)(uintptr_t)c->arg[1]);
            const u8 *bank = (const u8 *)(uintptr_t)c->arg[3];
            AddBank(bank);
            Refresh();
            a[3] = Magic(bank, "SBNK") && InHeap(bank) ? HeapDs(bank) : 0;
            break;
        }
        case SND_COMMAND_SETUP_CHANNEL_PCM: {
            u32 loopStart = (u32)c->arg[3] & 0xFFFF, loopLen = (u32)c->arg[2] & 0x3FFFFF;
            u32 len = (loopStart + loopLen) * 4;
            const u8 *d = (const u8 *)(uintptr_t)c->arg[1];
            if (d && len && len <= 0x80000) {
                a[1] = InHeap(d) ? HeapDs(d) : MapHost(d, len);
                Mem(a[1], d, len);
            } else {
                a[1] = 0;
            }
            break;
        }
        case SND_COMMAND_SETUP_CAPTURE: {
            const u8 *d = (const u8 *)(uintptr_t)c->arg[0];
            a[0] = d ? MapHost(d, (u32)c->arg[1] * 4 > 0x10000 ? 0x10000 : (u32)c->arg[1] * 4) : 0;
            break;
        }
        case SND_COMMAND_SHARED_WORK: {
            const u8 *d = (const u8 *)(uintptr_t)c->arg[0];
            if (d) Mem(DS_SHARED, d, sizeof(SNDSharedWork));
            if (live) acglue_shared((void *)d);
            a[0] = DS_SHARED;
            break;
        }
        case SND_COMMAND_READ_DRIVER_INFO:
            if (live) acglue_info((void *)(uintptr_t)c->arg[0]);
            a[0] = DS_INFO;
            break;
        case SND_COMMAND_INVALIDATE_SEQ:
        case SND_COMMAND_INVALIDATE_BANK:
        case SND_COMMAND_INVALIDATE_WAVE: {
            const u8 *s = (const u8 *)(uintptr_t)c->arg[0], *e = (const u8 *)(uintptr_t)c->arg[1];
            for (int i = 0; i < sNBanks; i++) {
                if ((const u8 *)sBanks[i] >= s && (const u8 *)sBanks[i] < e) sBanks[i--] = sBanks[--sNBanks];
            }
            a[0] = Tr(c->arg[0]), a[1] = Tr(c->arg[1]);
            break;
        }
        default:
            break;
        }
        if (lv) {
            lv[kl][0] = (u32)c->id;
            for (int j = 0; j < 4; j++) lv[kl][1 + j] = a[j];
            kl++;
        }
        if (!keep) continue;
        u8 *r = cmds + 16 + 20 * k;
        W32(r, (u32)c->id);
        for (int j = 0; j < 4; j++) W32(r + 4 + 4 * j, a[j]);
        k++;
    }
    if (n) Chunk("CMDS", cmds, 16 + 20 * (u32)n, NULL, 0);
    free(cmds);
    if (lv) {
        acglue_cmds((const u32(*)[5])lv, (u32)kl);
        free(lv);
    }
    sNLists++, sNCmds += (u64)n;
}

// the live path (acore_glue.c) wants the hook with or without BWPORT_ACAPTURE
void acapture_hook_install(void) {
    SIM_sndCaptureHook = Hook;
}

__attribute__((constructor)) static void AcaptureInit(void) {
    const char *e = getenv("BWPORT_ACAPTURE");
    if (!e || !*e) return;
    char path[1024];
    snprintf(path, sizeof path, "%s", e);
    char *comma = strchr(path, ',');
    if (comma) {
        *comma++ = 0;
        unsigned a, b;
        if (sscanf(comma, "%u-%u", &a, &b) == 2) sFrom = a, sTo = b;
        else if (sscanf(comma, "%u", &b) == 1) sTo = b;
    }
    sOut = fopen(path, "wb");
    if (!sOut) {
        fprintf(stderr, "acapture: cannot write %s\n", path);
        return;
    }
    static const u8 head[8] = {'A', 'C', 'S', 'T', 1, 0, 0, 0};
    fwrite(head, 1, 8, sOut);
    Info();
    atexit(Close);
    SIM_sndCaptureHook = Hook;
}
