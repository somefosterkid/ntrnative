#ifndef POKEHEARTGOLD_CODE_ENTRY_ARGS_H
#define POKEHEARTGOLD_CODE_ENTRY_ARGS_H

// CodeEntryArgs (hk290): the friend-code entry app's arguments, built by sub_02085400 (src/unk_020850F4_02085400_02085400.c,
// which still names it UnkStruct_02089438 after Platinum's twin), run as overlay _02102620, freed by sub_02085438.

#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"

typedef struct CodeEntryArgs {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C; // the code entered
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} CodeEntryArgs;

// sizeof(CodeEntryArgs) == 0x34

CodeEntryArgs *sub_02085400(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5);
void sub_02085438(CodeEntryArgs *args);

extern const OverlayManagerTemplate _02102620;

#endif // POKEHEARTGOLD_CODE_ENTRY_ARGS_H
