#ifndef POKEHEARTGOLD_UNK_STRUCT_0209_C1EC_H
#define POKEHEARTGOLD_UNK_STRUCT_0209_C1EC_H

#include "global.h"
#include "unk_struct_0209_c194.h"

// UnkStruct_0209C1EC: the script-launched app task (sub_0209707C, HEAP_ID_FIELD2, ScrCmd_629 -> sub_020970C0). unk_00
// indexes the 6 state handlers _02108EC0; unk_08 is the 7-word args that sub_02097024 copies into the
// UnkStruct_0209C194 block at unk_24.

struct FieldSystem;
struct PartyMenuArgs;
struct PokemonSummaryArgs;

typedef struct UnkStruct_0209C1EC {
    int unk_00;
    int unk_04;
    UnkStruct_0209C194_1 unk_08;
    UnkStruct_0209C194 *unk_24;
    struct FieldSystem *fieldSystem;
    struct PartyMenuArgs *partyMenu;
    struct PokemonSummaryArgs *monSummary;
} UnkStruct_0209C1EC;

// sizeof(UnkStruct_0209C1EC) == 0x34 on the DS

#endif // POKEHEARTGOLD_UNK_STRUCT_0209_C1EC_H
