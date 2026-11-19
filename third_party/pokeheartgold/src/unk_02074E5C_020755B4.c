#include "global.h"

#include "constants/heap.h"

#include "save_palpad.h"
#include "unk_02035900.h"

typedef struct UnkStruct_sub_020755B4_commState {
    void *unk_0;
    u8 unk_4[0x101C];
    u8 unk_1020;
} UnkStruct_sub_020755B4_commState;

void sub_020755B4(s32 param0, s32 param1, void *param2, UnkStruct_sub_020755B4_commState *commState);

int sub_020755E4(void);

void sub_020755B4(s32 param0, s32 param1, void *param2, UnkStruct_sub_020755B4_commState *commState) {
    if (param0 != sub_0203769C()) {
        SavePalPad_Merge(*(SavePalPad **)((u8 *)(commState->unk_0) + 0x148), (SavePalPad *)param2, 1, HEAP_ID_BATTLE);
    }
    commState->unk_1020 = (u8)(commState->unk_1020 + 1);
}

int sub_020755E4(void) {
    return 136;
}
