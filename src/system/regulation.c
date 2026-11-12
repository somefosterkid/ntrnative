// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/regulation.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include <stddef.h>
#include "types.h"
#include "battle/regulation.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "stddef.h"

// The rules of a battle, and the save block of the competitions' regulations and the teams entered in them, which
// swan calls the tournament block. Block 0x2f.

#define SAVE_BLOCK_TOURNAMENT 0x2f

#define REGULATION_NAME_STRBUF_SIZE 26

void loadBattleTypeTextToStrbuf(Regulation *regulation, StrBuf *strbuf);
static void func_0200b254(TournamentTeam *team, u32 field, u8 index, u32 value);

u32 getSizeofTourneyBlock(void) {
    return sizeof(TournamentBlock);
}

// pokebw2's Regulation_Create
Regulation *func_0200adf0(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(Regulation), TRUE, "regulation.c", 115);
}

// pokebw2's Regulation_Copy
void func_0200ae08(const Regulation *src, Regulation *dest) {
    MI_CpuCopy8(src, dest, sizeof(Regulation));
}

Regulation *func_0200ae14(void *entry) {
    return entry;
}

void loadBattleTypeTextToStrbuf(Regulation *regulation, StrBuf *strbuf) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_LOAD_BATTLE_TYPE_TEXT_TO_STRBUF, HEAPID_USER);

    GFL_MsgDataLoadStrbuf(msgData, regulation->unk0[0], strbuf);
    GFL_MsgDataFree(msgData);
}


StrBuf *makeStrbufLoadBattleTypeText(Regulation *regulation, HeapID heapId) {
    StrBuf *strbuf = GFL_StrBufCreate(REGULATION_NAME_STRBUF_SIZE, heapId);

    loadBattleTypeTextToStrbuf(regulation, strbuf);
    return strbuf;
}

u32 func_0200ae54(Regulation *regulation, u32 param) {
    u32 value = 0;

    switch (param) {
    case REGULATION_PARAM_NAME:
        value = regulation->unk0[0];
        break;
    case REGULATION_PARAM_1:
        value = regulation->unk0[1];
        break;
    case REGULATION_PARAM_2:
        value = regulation->unk2;
        break;
    case REGULATION_PARAM_3:
        value = regulation->unk3;
        break;
    case REGULATION_PARAM_4:
        value = regulation->unk4;
        break;
    case REGULATION_PARAM_5:
        value = regulation->unk5;
        break;
    case REGULATION_PARAM_6:
        value = regulation->unk6;
        break;
    case REGULATION_PARAM_7:
        value = regulation->unk8;
        break;
    case REGULATION_PARAM_8:
        value = regulation->unk9;
        break;
    case REGULATION_PARAM_BITS_A:
    case REGULATION_PARAM_BITS_5C:
        break;
    case REGULATION_PARAM_11:
        value = regulation->unkA8;
        break;
    case REGULATION_PARAM_12:
        value = regulation->unkAA;
        break;
    case REGULATION_PARAM_13:
        value = regulation->unkAB;
        break;
    case REGULATION_PARAM_BITS_AC:
        break;
    case REGULATION_PARAM_15:
        value = regulation->unkB3;
        break;
    case REGULATION_PARAM_16:
        value = regulation->unkB4;
        break;
    case REGULATION_PARAM_17:
        value = regulation->unkB5;
        break;
    case REGULATION_PARAM_18:
        value = regulation->unkB6;
        break;
    case REGULATION_PARAM_SHOW_PARTNERS:
        value = regulation->showPartners;
        break;
    case REGULATION_PARAM_TIME_LIMIT:
        value = regulation->timeLimit;
        break;
    case REGULATION_PARAM_BATTLE_TYPE:
        value = regulation->unkBA;
        break;
    case REGULATION_PARAM_22:
        value = regulation->unkBB;
        break;
    }
    return value;
}

void func_0200aee8(Regulation *regulation, u32 param, int value) {
    switch (param) {
    case REGULATION_PARAM_NAME:
        if (value < 7) {
            regulation->unk0[0] = value;
        }
        break;
    case REGULATION_PARAM_1:
        if (value < 6) {
            regulation->unk0[1] = value;
        }
        break;
    case REGULATION_PARAM_2:
        if (value < 6) {
            regulation->unk2 = value;
        }
        break;
    case REGULATION_PARAM_3:
        if (value < 6) {
            regulation->unk3 = value;
        }
        break;
    case REGULATION_PARAM_4:
        if (value <= 100) {
            regulation->unk4 = value;
        }
        break;
    case REGULATION_PARAM_5:
        if (value <= 6) {
            regulation->unk5 = value;
        }
        break;
    case REGULATION_PARAM_6:
        if (value <= 600) {
            regulation->unk6 = value;
        }
        break;
    case REGULATION_PARAM_7:
        if (value < 2) {
            regulation->unk8 = value;
        }
        break;
    case REGULATION_PARAM_8:
        if (value < 2) {
            regulation->unk9 = value;
        }
        break;
    case REGULATION_PARAM_11:
        if (value < 651) {
            regulation->unkA8 = value;
        }
        break;
    case REGULATION_PARAM_12:
        regulation->unkAA = value;
        break;
    case REGULATION_PARAM_13:
        if (value < 2) {
            regulation->unkAB = value;
        }
        break;
    case REGULATION_PARAM_15:
        regulation->unkB3 = value;
        break;
    case REGULATION_PARAM_16:
        regulation->unkB4 = value;
        break;
    case REGULATION_PARAM_17:
        if (value < 2) {
            regulation->unkB5 = value;
        }
        break;
    case REGULATION_PARAM_18:
        regulation->unkB6 = value;
        break;
    case REGULATION_PARAM_SHOW_PARTNERS:
        if (value < 2) {
            regulation->showPartners = value;
        }
        break;
    case REGULATION_PARAM_TIME_LIMIT:
        if (value <= 99) {
            regulation->timeLimit = value;
        }
        break;
    case REGULATION_PARAM_BATTLE_TYPE:
        if (value < 6) {
            regulation->unkBA = value;
        }
        break;
    case REGULATION_PARAM_22:
        regulation->unkBB = value;
        break;
    }
}

BOOL func_0200afd4(Regulation *regulation, u32 param, int index) {
    int byte = index / 8;
    int bit = index % 8;

    switch (param) {
    case REGULATION_PARAM_BITS_A:
        if (regulation->unkA[byte] & (1 << bit)) {
            return TRUE;
        }
        break;
    case REGULATION_PARAM_BITS_5C:
        if (regulation->unk5C[byte] & (1 << bit)) {
            return TRUE;
        }
        break;
    case REGULATION_PARAM_BITS_AC:
        if (regulation->unkAC[byte] & (1 << bit)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_0200b040(Regulation *regulation, RegulationUnkAC *out) {
    MI_CpuFill8(out, 0, 8);
    out->unk7 = func_0200ae54(regulation, REGULATION_PARAM_13);
    if (out->unk7 >= 2) {
        out->unk7 = 1;
    }
    MI_CpuCopy8(regulation->unkAC, out->unk0, 7);
}





void *func_0200b074(void *entry) {
    TournamentEntry *tournament = entry;

    return tournament->unkC0;
}

// pokebw2's TournamentEntry_GetField
u32 func_0200b078(void *entry, u32 field) {
    TournamentEntry *tournament = entry;
    u32 value;

    switch (field) {
    case TOURNAMENT_FIELD_UNKBC:
        value = tournament->unkBC;
        break;
    case TOURNAMENT_FIELD_UNK152:
        value = tournament->unk152;
        break;
    case TOURNAMENT_FIELD_UNK154:
        value = tournament->unk154;
        break;
    case TOURNAMENT_FIELD_UNK155:
        value = tournament->unk155;
        break;
    case TOURNAMENT_FIELD_UNK156:
        value = tournament->unk156;
        break;
    case TOURNAMENT_FIELD_UNK157:
        value = tournament->unk157;
        break;
    case TOURNAMENT_FIELD_UNK158:
        value = tournament->unk158;
        break;
    case TOURNAMENT_FIELD_UNK159:
        value = tournament->unk159;
        break;
    case TOURNAMENT_FIELD_UNK15A:
        value = tournament->unk15A;
        break;
    case TOURNAMENT_FIELD_UNK15B:
        value = tournament->unk15B;
        break;
    case TOURNAMENT_FIELD_UNK15C:
        value = tournament->unk15C;
        break;
    case TOURNAMENT_FIELD_UNK15D:
        value = tournament->unk15D;
        break;
    }
    return value;
}

// pokebw2's TournamentEntry_SetField
void func_0200b110(void *entry, u32 field, u32 value) {
    TournamentEntry *tournament = entry;

    switch (field) {
    case TOURNAMENT_FIELD_UNKBC:
        tournament->unkBC = value;
        break;
    case TOURNAMENT_FIELD_UNK152:
        tournament->unk152 = value;
        break;
    case TOURNAMENT_FIELD_UNK154:
        tournament->unk154 = value;
        break;
    case TOURNAMENT_FIELD_UNK155:
        tournament->unk155 = value;
        break;
    case TOURNAMENT_FIELD_UNK156:
        tournament->unk156 = value;
        break;
    case TOURNAMENT_FIELD_UNK157:
        tournament->unk157 = value;
        break;
    case TOURNAMENT_FIELD_UNK158:
        tournament->unk158 = value;
        break;
    case TOURNAMENT_FIELD_UNK159:
        tournament->unk159 = value;
        break;
    case TOURNAMENT_FIELD_UNK15A:
        tournament->unk15A = value;
        break;
    case TOURNAMENT_FIELD_UNK15B:
        tournament->unk15B = value;
        break;
    case TOURNAMENT_FIELD_UNK15C:
        tournament->unk15C = value;
        break;
    case TOURNAMENT_FIELD_UNK15D:
        tournament->unk15D = value;
        break;
    }
}

// pokebw2's TournamentEntry_CheckCRC
BOOL func_0200b1a8(void *entry) {
    TournamentEntry *tournament = entry;

    if (tournament->crc == CRC16_350bytes(tournament)) {
        return TRUE;
    }
    return FALSE;
}

u16 CRC16_350bytes(const void *entry) {
    return getCRC16(entry, offsetof(TournamentEntry, crc));
}

// pokebw2's TournamentBlock_GetEntry
void *func_0200b1d4(void *block, u32 index) {
    TournamentBlock *tournament = block;

    return &tournament->entries[index];
}

// pokebw2's TournamentBlock_SetEntry
void func_0200b1e0(void *block, u32 index, const void *entry) {
    TournamentBlock *tournament = block;

    tournament->entries[index] = *(const TournamentEntry *)entry;
}

// pokebw2's TournamentTeam_Clear
void func_0200b1f8(void *record) {
    MI_CpuFill8(record, 0, sizeof(TournamentTeam));
}

// pokebw2's TournamentBlock_GetTeam
void *func_0200b204(void *block, u32 index) {
    TournamentBlock *tournament = block;

    return &tournament->teams[index];
}

// pokebw2's TournamentTeam_GetField
u32 func_0200b214(void *record, u32 field, u8 index) {
    TournamentTeam *team = record;

    switch (field) {
    case TOURNAMENT_TEAM_SPECIES:
        return team->species[index];
    case TOURNAMENT_TEAM_FORM:
        return team->form[index];
    case TOURNAMENT_TEAM_HAS_ITEM:
        return (team->hasItem >> index) & 1;
    case TOURNAMENT_TEAM_GENDER:
        return (team->gender >> index) & 1;
    }
    return 0;
}

// pokebw2's TournamentTeam_SetField
static void func_0200b254(TournamentTeam *team, u32 field, u8 index, u32 value) {
    switch (field) {
    case TOURNAMENT_TEAM_SPECIES:
        team->species[index] = value;
        break;
    case TOURNAMENT_TEAM_FORM:
        team->form[index] = value;
        break;
    case TOURNAMENT_TEAM_HAS_ITEM:
        team->hasItem |= (value == 1) << index;
        break;
    case TOURNAMENT_TEAM_GENDER:
        team->gender |= (value == 1) << index;
        break;
    }
}

// pokebw2's TournamentTeam_SetFromParty
void func_0200b2a4(void *record, PokeParty *party) {
    s32 i;

    func_0200b1f8(record);
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            func_0200b254(record, TOURNAMENT_TEAM_SPECIES, i, PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL));
            func_0200b254(record, TOURNAMENT_TEAM_FORM, i, PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL));
            func_0200b254(record, TOURNAMENT_TEAM_HAS_ITEM, i, PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL) != 0);
            func_0200b254(record, TOURNAMENT_TEAM_GENDER, i, PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL));
        }
    }
}

void clearTournamentBlock(void *block) {
    MI_CpuFill8(block, 0, sizeof(TournamentBlock));
}

void *getTournamentBlockAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_TOURNAMENT);
}
