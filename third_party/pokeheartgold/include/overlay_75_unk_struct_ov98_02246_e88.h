#ifndef POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02246_E88_H
#define POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02246_E88_H

// UnkStruct_ov98_02246E88: merged by tools/hg/promote.py from 27 m2c local copies (1 variants, base
// src/overlay_75_022469D8.c) and 12 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct NNSiFndHeapHead;
struct Options;
struct SaveData;
struct String;

typedef struct UnkStruct_ov98_02246E88 {
    struct ApplicationManager *appMan;
    struct SaveData *saveData;
    struct Options *options;
    void *unk_0C;
    struct NNSiFndHeapHead *unk_10;
    struct { int ac_state; u16 state; u16 online; u16 dmaNo; u16 powerMode; u8 wontExecuteWDS; u8 unk_10_pad[0x3]; u32 processWDSstate; struct { u8 ssid[32]; u8 wep[32]; int wepMode; s8 apnum[10]; u8 unk_4E_tail[0x2]; } nspotInfo; } unk_14;
    u16 unk_7C;
    u8 unk_7E;
    u8 padding_7F;
    int unk_80;
    int unk_84;
    u8 padding_88[4];
    int unk_8C;
    int unk_90;
    u8 padding_94[4];
    void *unk_98;
    struct { u8 gameCode; u8 language; u8 country; u8 unk_03; u32 trainerId; u16 trainerName[8]; u32 unk_10; u8 macAddress[6]; u8 unk_1A[2]; s8 email[56]; u32 emailInitialised; u16 rngValue; u16 unk_5A; } unk_9C;
    struct { u32 validationError; u32 systemError; } unk_100;
    u32 unk_108;
    u32 unk_10C;
    u8 unk_110;
    u8 unk_111[3];
    struct String *unk_114;
    struct String *unk_118;
    BOOL unk_11C;
} UnkStruct_ov98_02246E88;

// sizeof(UnkStruct_ov98_02246E88) == 0x11c

#endif // POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02246_E88_H
