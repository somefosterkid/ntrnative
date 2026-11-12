// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/snd_bgm_stack.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/sound.h"
#include "nnsys/snd.h"

// The BGM stack: each level holds a BGM's handle and sequence, and the sound heap's levels before and after its files
// were loaded, so that pushing a BGM frees the waves of the one below and popping it loads them again. The file's
// name is descriptive, a guess: the ROM has no string for it. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except func_020057e8, func_02005834 and
// func_02005858. The data's names, types, constants and fields are ours

#define BGM_STACK_SIZE 6
#define BGM_STACK_NO_SEQ 0xffff

typedef struct {
    BOOL active;
    u16 seq;
    NNSSndHandle handle;
    // The sound heap's level when the entry was made, before its BGM, after its sequence and bank, and after its
    // waves
    s32 baseLevel;
    s32 bgmLevel;
    s32 seqLevel;
    s32 waveLevel;
} BGMStackEntry;

typedef struct {
    u16 player;
    s32 depth;
    NNSSndHeapHandle *heap;
} BGMStack;

// What func_02005a98 preloads, in the sound heap, and gives back for func_02005b04: the count of sequences, and the
// level to go back to
struct BGMStackPreload {
    u32 count;
    s32 level;
};

static void func_020057e8(s32 depth);
static void func_02005834(s32 level);
static void func_02005858(void);

// pokebw2's sBGMStack
static BGMStack data_020aa1ac;
// pokebw2's sBGMStackEntries
static BGMStackEntry data_020aa1b8[BGM_STACK_SIZE];

void func_020057dc(NNSSndHeapHandle *heap) {
    data_020aa1ac.heap = heap;
}

// pokebw2's BGMStack_ResetEntry
static void func_020057e8(s32 depth) {
    data_020aa1b8[depth].active = FALSE;
    data_020aa1b8[depth].seq = BGM_STACK_NO_SEQ;
    NNS_SndHandleReleaseSeq(&data_020aa1b8[depth].handle);
    data_020aa1b8[depth].baseLevel = -1;
    data_020aa1b8[depth].bgmLevel = -1;
    data_020aa1b8[depth].seqLevel = -1;
    data_020aa1b8[depth].waveLevel = -1;
}

// pokebw2's BGMStack_InitEntry
static void func_02005834(s32 level) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    entry->baseLevel = level;
    entry->bgmLevel = level;
    entry->seqLevel = level;
    entry->waveLevel = level;
    data_020aa1b8[data_020aa1ac.depth].active = TRUE;
}

// pokebw2's BGMStack_FreeEntry
static void func_02005858(void) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    NNS_SndHandleReleaseSeq(&entry->handle);
    NNS_SndHeapLoadState(*data_020aa1ac.heap, entry->baseLevel);
    func_020057e8(data_020aa1ac.depth);
}

void func_02005888(u16 player) {
    s32 i;

    data_020aa1ac.player = player;
    data_020aa1ac.depth = 0;
    for (i = 0; i < BGM_STACK_SIZE; i++) {
        func_020057e8(i);
        NNS_SndHandleInit(&data_020aa1b8[i].handle);
    }
    NNS_SndPlayerSetPlayableSeqCount(data_020aa1ac.player, BGM_STACK_SIZE);
    func_02005834(NNS_SndHeapSaveState(*data_020aa1ac.heap));
}

u32 GFL_SndGetLastResID(void) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    if (data_020aa1b8[data_020aa1ac.depth].active == FALSE) {
        return 0;
    }
    if (entry->seq == BGM_STACK_NO_SEQ) {
        return 0;
    }
    return entry->seq;
}

s32 func_02005900(void) {
    return data_020aa1ac.depth;
}

NNSSndHandle *func_0200590c(void) {
    return &data_020aa1b8[data_020aa1ac.depth].handle;
}

BOOL func_02005924(u32 seq) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];
    BOOL result;

    if (NNS_SndArcLoadSeqEx(seq, NNS_SND_ARC_LOAD_SEQ | NNS_SND_ARC_LOAD_BANK, *data_020aa1ac.heap) == FALSE) {
        return FALSE;
    }
    entry->seqLevel = NNS_SndHeapSaveState(*data_020aa1ac.heap);
    result = NNS_SndArcLoadSeqEx(seq, NNS_SND_ARC_LOAD_WAVE, *data_020aa1ac.heap);
    if (result == FALSE) {
        return FALSE;
    }
    entry->waveLevel = NNS_SndHeapSaveState(*data_020aa1ac.heap);
    entry->seq = seq;
    return result;
}

void func_0200597c(void) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    entry->seqLevel = NNS_SndHeapSaveState(*data_020aa1ac.heap);
}

BOOL func_020059a0(u32 seq) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    entry->waveLevel = NNS_SndHeapSaveState(*data_020aa1ac.heap);
    entry->seq = seq;
    return TRUE;
}

void func_020059c8(void) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];

    if (data_020aa1b8[data_020aa1ac.depth].active) {
        entry->seq = BGM_STACK_NO_SEQ;
        NNS_SndHeapLoadState(*data_020aa1ac.heap, entry->bgmLevel);
        entry->seqLevel = entry->bgmLevel;
        entry->waveLevel = entry->bgmLevel;
    }
}

BOOL func_02005a00(void) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];
    s32 level;

    if (data_020aa1ac.depth >= BGM_STACK_SIZE - 1) {
        return FALSE;
    }
    if (entry->seqLevel != -1) {
        NNS_SndHeapLoadState(*data_020aa1ac.heap, entry->seqLevel);
        entry->waveLevel = -1;
        level = entry->seqLevel;
    } else {
        level = entry->bgmLevel;
    }
    data_020aa1ac.depth++;
    func_02005834(level);
    return TRUE;
}

BOOL func_02005a4c(void) {
    BGMStackEntry *entry;

    if (data_020aa1ac.depth <= 0) {
        return FALSE;
    }
    func_02005858();
    data_020aa1ac.depth--;
    entry = &data_020aa1b8[data_020aa1ac.depth];
    if (entry->seq != BGM_STACK_NO_SEQ) {
        NNS_SndArcLoadSeqEx(entry->seq, NNS_SND_ARC_LOAD_BANK | NNS_SND_ARC_LOAD_WAVE, *data_020aa1ac.heap);
        entry->waveLevel = NNS_SndHeapSaveState(*data_020aa1ac.heap);
    }
    return TRUE;
}

BGMStackPreload *func_02005a98(const u32 *seqs, u32 count) {
    BGMStackEntry *entry = &data_020aa1b8[data_020aa1ac.depth];
    BGMStackPreload *preload;
    s32 level = entry->baseLevel;
    u32 i;

    func_02005858();
    preload = NNS_SndHeapAlloc(*data_020aa1ac.heap, sizeof(BGMStackPreload), NULL, 0, 0);
    for (i = 0; i < count; i++) {
        NNS_SndArcLoadSeq(seqs[i], *data_020aa1ac.heap);
    }
    preload->count = count;
    preload->level = level;
    func_02005834(NNS_SndHeapSaveState(*data_020aa1ac.heap));
    return preload;
}

void func_02005b04(BGMStackPreload *preload) {
    s32 level = preload->level;

    func_02005858();
    NNS_SndHeapLoadState(*data_020aa1ac.heap, level);
    func_02005834(level);
}
