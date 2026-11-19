#ifndef POKEHEARTGOLD_UNK_02037C94_WORK_H
#define POKEHEARTGOLD_UNK_02037C94_WORK_H

// CommStateWork (formerly the local UnkStruct_sub_02039C6C of unk_02037C94.c): the comm layer's state machine work, allocated
// by sub_02037C98 (and sub_02039418 / sub_020394A0 / sub_02039528) from HEAP_ID_15 and held in _021D4150. Field names are the
// DS offsets; the pointer fields (unk0, unk24, unk28, unk2C) widen on the PC, so every access is by field.
// sizeof(CommStateWork) == 0x68 on the DS

#include "global.h"

#include "overlay_44_02232E9C.h"
#include "player_data.h"
#include "save.h"
#include "save_link_ruleset.h"

typedef struct CommStateWork {
    UnkStruct_ov44_0223197C *unk0;
    u8 unk4[8];
    MATHRandContext32 mathRandContext;
    void (*unk24)(void);
    SaveData *unk28;
    PlayerProfile *unk2C;
    const LinkBattleRuleset *unk30;
    u32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u16 unk48;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E;
    u8 unk4F;
    u8 unk50;
    s8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 unk59[3];
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 unk5F;
    s32 unk60;
    u8 unk64;
} CommStateWork;

extern CommStateWork *_021D4150;

#endif // POKEHEARTGOLD_UNK_02037C94_WORK_H
