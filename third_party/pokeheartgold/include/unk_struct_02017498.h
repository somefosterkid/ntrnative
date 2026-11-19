#ifndef POKEHEARTGOLD_UNK_STRUCT_02017498_H
#define POKEHEARTGOLD_UNK_STRUCT_02017498_H

// UnkStruct_02017498: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/unk_020192D0_0201935C.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct NNSiFndHeapHead;

typedef struct UnkStruct_02017498 {
    void *unk_00;
    BOOL unk_10;
    struct ApplicationManager *appMan;
    void *unk_18;
    struct NNSiFndHeapHead *unk_1C;
    struct { int ac_state; u16 state; u16 online; u16 dmaNo; u16 powerMode; u8 wontExecuteWDS; u8 unk_10_pad[0x3]; u32 processWDSstate; struct { u8 ssid[32]; u8 wep[32]; int wepMode; s8 apnum[10]; u8 unk_4E_tail[0x2]; } nspotInfo; } unk_20;
    int unk_88;
    BOOL unk_8C;
} UnkStruct_02017498;

// sizeof(UnkStruct_02017498) == 0x80

#endif // POKEHEARTGOLD_UNK_STRUCT_02017498_H
