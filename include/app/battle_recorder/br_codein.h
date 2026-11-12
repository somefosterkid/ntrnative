// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_codein.h: the lines
// in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_H
#define BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The groups the digits are shown in, the last one repeated after them
#define BR_CODEIN_BLOCK_COUNT 3

// Black 2's; twinned code confirms the values of BR_CODEIN_RESULT_CANCEL and BR_CODEIN_RESULT_DECIDE in Black
// What the player did
enum {
    BR_CODEIN_RESULT_NONE,
    // Went back with no digit left to erase
    BR_CODEIN_RESULT_CANCEL,
    BR_CODEIN_RESULT_DECIDE,
};

typedef struct BrCodeInWork BrCodeInWork;

// pokebw2's BrCodeInParam_Create
// blocks holds the number of digits of each group
BrCodeInParam *func_ov183_021e07a4(u32 heapId, u32 length, ClActUnit *unit, BrRes *res, BrBallEffect *ballEff,
                                    const int *blocks);
// pokebw2's BrCodeIn_Exit
void func_ov183_021e0774(BrCodeInWork *wk);
// pokebw2's BrCodeIn_GetResult
// BR_CODEIN_RESULT_*
int func_ov183_021e075c(const BrCodeInWork *wk);
// pokebw2's BrCodeIn_GetNumber
// The number typed, once the result is BR_CODEIN_RESULT_DECIDE
u64 func_ov183_021e0764(const BrCodeInWork *wk);

// Black 2's; matching code confirms the offsets of length, blocks, strbuf, unit, res and ballEff, and its size in Black
struct BrCodeInParam {
    // How many digits the code has
    u32 length;
    int blocks[BR_CODEIN_BLOCK_COUNT + 1];
    u32 unk14;
    // The code typed, length characters. The original's name, from BrCodeInParam_Free's assertions
    StrBuf *strbuf;
    ClActUnit *unit;
    BrRes *res;
    // Shown where the player touches a key
    BrBallEffect *ballEff;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrCodeIn_Init
BrCodeInWork *func_ov183_021e06d0(const BrCodeInParam *param, HeapID heapId);
// pokebw2's BrCodeInParam_Free
void func_ov183_021e0808(BrCodeInParam *codein_param);
// pokebw2's BrCodeIn_Main
void func_ov183_021e0754(BrCodeInWork *wk);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_H
