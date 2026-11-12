// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_setup.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_SETUP_H
#define BWPORT_BATTLE_BTL_SETUP_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/math.h"
#include "struct_decls.h"
#include "system/pms_data.h"

// The background, place and time of a battle, which the battle view copies as one struct
typedef struct {
    u32 bgType;
    u32 terrain;
    u8 weather;
    u8 season;
    u16 zoneId;
    u8 hour;
    u8 minute;
} BtlFieldEnv;

// The battle's surroundings, as in Black 2
struct BtlFieldSituation {
    BtlFieldEnv env;
    // The battle's music
    u16 bgm;
    u16 unk12;
    void *netHandle;
    u8 unk18;
    u8 unk19;
    u8 unk1a;
    u8 unk1b;
};

// Black's, 0xf0 bytes, from btl_setup.c's code: Black 2's (0x13c) without the trainers' names, from gameData on
// 0x18 bytes earlier, with 16-bit flags, fewer fields after the random state and a shorter end. Fields named by
// Black's offsets
struct BtlSetup {
    u32 battleType;
    u32 battleStyle;
    BtlFieldSituation fieldSituation;
    // The parties of the four clients
    PokeParty *party[4];
    PlayerInfo *unk34[4];
    u8 unk44[4];
    // The trainers of the four clients
    BtlSetupTrainer *trainers[4];
    GameData *gameData;
    Config *config;
    BagSave *bag;
    void *unk64;
    PokeDexSave *pokedex;
    GameRecords *records;
    // GameData_GetCommPlayerSupport's
    void *unk70;
    u16 unk74;
    u16 unk76;
    // What func_0200b760 fills in from a regulation, a RegulationUnkAC
    u8 unk78[7];
    u8 unk7F;
    // The player's badges
    u8 unk80;
    u8 unk81;
    // BtlSetup_SetFlag sets these
    u16 flags;
    u16 unk84;
    u16 unk86;
    u32 unk88;
    u32 unk8C;
    u8 unk90;
    u8 unk91;
    u8 unk92;
    u8 unk93;
    void *unk94;
    u32 unk98;
    // The battle's random state, which the main module copies back
    MATHRandContext32 rand;
    u16 unkB4;
    u8 unkB6;
    u8 unkB7;
    u8 unkB8;
    u8 unkB9;
    u8 unkBA;
    u8 unkBB;
    u8 unkBC;
    u8 unkBD;
    u8 unkBE;
    u8 unkBF;
    u8 unkC0;
    u8 unkC1_0 : 1;
    u8 unkC1_1 : 1;
    u8 unkC1_2 : 1;
    u8 unkC1_3 : 2;
    u8 unkC1_5 : 1;
    u8 unkC1_6 : 1;
    u8 unkC1_7 : 1;
    u8 unkC2[6];
    // The party slots each client sends out
    u8 unkC8[4][6];
    u32 unkE0[4];
};

void BtlSetup_Free(BtlSetup *setup);
PokeParty *BtlSetup_GetParty(BtlSetup *setup, u32 index);
// Link battles
void BtlSetup_SetNet1v1Single(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, HeapID heapId);
void BtlSetup_SetNet1v1Double(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, HeapID heapId);
void BtlSetup_SetNetMultiVsNet(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, u8 a4, HeapID heapId);
void BtlSetup_SetNetTriple(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, HeapID heapId);
void BtlSetup_SetNetRotation(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, HeapID heapId);
// pokebw2's func_02017cac, func_02017cfc, func_02017d30 and func_020186b0
void func_020130dc(BtlSetup *setup);
void func_02013128(BtlSetup *setup, PokeParty *party, u32 a2);
void func_02013160(BtlSetup *setup, Regulation *regulation, HeapID heapId);
// pokebw2's BtlSetup_PostProcessTrialHouse
void func_0201394c(BtlSetup *setup);
void func_02013980(BtlSetup *setup, HeapID heapId);
// pokebw2's func_0201f63c
void func_0201a2a8(Regulation *regulation, PokeParty *party);

// A trainer the battle is set up with
struct BtlSetupTrainer {
    u32 trainerId;
    u32 trainerClass;
    u32 aiFlags;
    u16 items[4];
    StrBuf *name;
    PMSData unk18;
    PMSData unk20;
};

// Wild battles, against the party, alone or beside the trainer
void BtlSetup_SetWildNormal(BtlSetup *setup, GameData *gameData, PokeParty *party, BtlFieldStatus *status,
                            BOOL doubleBattle, HeapID heapId);
// Battles against trainers, in the surroundings that SaveBtlFieldStatus saved
void BtlSetup_SetTrainer1v1Single(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 a3, HeapID heapId);
void BtlSetup_SetTrainer1v1Double(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 a3, HeapID heapId);
void BtlSetup_SetTrainer2v2(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 a3, u32 a4, u32 a5,
                            HeapID heapId);
void BtlSetup_SetTrainer1v2(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 trainerId, u32 trainerId2,
                             HeapID heapId);
void BtlSetup_SetTrainer3v3(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 a3, HeapID heapId);
void BtlSetup_SetTrainerRotation(BtlSetup *setup, GameData *gameData, BtlFieldStatus *status, u32 a3, HeapID heapId);
// A multi battle of two linked players against trainers
void BtlSetup_SetNetMultiVsAI(BtlSetup *setup, GameData *gameData, NetHandle *handle, u8 a3, u8 a4, u32 a5, u32 a6,
                              HeapID heapId);
// The capture demonstration, between the two parties
void BtlSetup_SetCaptureDemo(BtlSetup *setup, GameData *gameData, PokeParty *party, PokeParty *enemyParty,
                             BtlFieldStatus *status, HeapID heapId);
// A field status with the default background
void ResetBtlFieldStatus(BtlFieldStatus *status);

void func_0200b400(HeapID heapId);

// Black 2's; twinned code confirms the offsets of trainerId, trainerClass, aiFlags, items, unk18, unk20, name and
// nameLength in Black
// A trainer as a link battle sends it, with the name as characters
struct BtlCommTrainerData {
    u32 trainerId;
    u32 trainerClass;
    u32 aiFlags;
    u16 items[4];
    u8 unk14[4];
    PMSData unk18;
    PMSData unk20;
    u16 name[0x20];
    u32 nameLength;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void freeVSPlayerBlkClearPtr(void);
BtlSetup *BtlSetup_Create(HeapID heapId);
u32 BtlSetup_CheckFlag(BtlSetup *setup, u32 flag);
void BtlSetup_Reset(BtlSetup *setup);
void BtlSetup_SetFlag(BtlSetup *setup, u32 flag);
// Gives a battle's trainer a name buffer and empty words
BtlSetupTrainer *TrainerBattleSetup_Create(HeapID heapId);
// pokebw2's BtlSetup_InitForRecord
void func_020138c4(BtlSetup *setup, GameData *gameData, HeapID heapId);
// pokebw2's BtlSetup_FreeForRecord
void func_02013930(BtlSetup *setup);

#endif // BWPORT_BATTLE_BTL_SETUP_H
