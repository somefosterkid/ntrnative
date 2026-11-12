// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/musical_stage_sys.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_MUSICAL_STAGE_SYS_H
#define BWPORT_FIELD_MUSICAL_STAGE_SYS_H

// Overlay 10's musical_stage_sys.c: the proc that runs the stage, and the Pokémon on it

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

struct MusicalStageParam {
    void *comm;
    MusicalPoke *pokes[4];
    MusicalProgram *program;
    Ov210Work *ov210;
    u32 unk1C;
};

// pokebw2's func_ov012_02152280 and func_ov012_021522bc
void func_ov010_02156fd0(MusicalStageParam *param, u8 pos, u16 species, u8 form, u32 personality, u16 a5,
                         HeapID heapId);
void func_ov010_0215700c(MusicalStageParam *param, u8 pos, u8 slot, u16 itemId, s16 a4, u8 a5);

// pokebw2's StaActing_Main
BOOL func_ov132_021f879c(StaActing *stage);
// pokebw2's StaActing_Term
void func_ov132_021f8640(StaActing *stage);
void func_ov010_02156f98(MusicalStageParam *param);
// Puts a Pokémon on the stage
void func_ov010_02156fc4(MusicalStageParam *param, u8 pos, MusicalPoke *poke);
void func_ov010_02157000(MusicalStageParam *param, u8 pos, MusicalPoke *poke);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
MusicalStageParam *func_ov010_02156f68(HeapID heapId, void *comm);
// pokebw2's StaActing_Init
StaActing *func_ov132_021f8480(MusicalStageParam *param, HeapID heapId);

extern GameProcFunctions data_ov010_02172748;

#endif // BWPORT_FIELD_MUSICAL_STAGE_SYS_H
