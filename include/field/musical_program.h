// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/musical_program.h.
#ifndef BWPORT_FIELD_MUSICAL_PROGRAM_H
#define BWPORT_FIELD_MUSICAL_PROGRAM_H

// Overlay 10's musical_program.c: the program of a show, which decides the points of each kind of prop and the
// Pokémon of the other performers

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

MusicalProgram *func_ov010_02157028(HeapID heapId, Ov210Work *ov210, u16 shots);
void func_ov010_0215716c(MusicalProgram *program);
void func_ov010_02157174(HeapID heapId, MusicalProgram *program, MusicalStageParam *stage);
u8 func_ov010_0215725c(MusicalProgram *program, u8 kind);
u32 func_ov010_02157260(MusicalProgram *program);
void func_ov010_02157278(MusicalProgram *program, u32 value);
u32 func_ov010_02157298(MusicalProgram *program);
void func_ov010_021572a8(MusicalProgram *program, u32 value);
u8 func_ov010_021572c0(MusicalProgram *program);
void func_ov010_021572e4(MusicalProgram *program, MusicalStageParam *stage, u8 pos, u8 cpu, HeapID heapId);
u8 func_ov010_02157364(MusicalProgram *program);
u8 func_ov010_0215736c(MusicalProgram *program);
u8 func_ov010_02157374(MusicalProgram *program, u8 cpu);
u8 func_ov010_02157384(MusicalProgram *program, u8 cpu);
u8 func_ov010_02157394(MusicalProgram *program, u8 kind);

#endif // BWPORT_FIELD_MUSICAL_PROGRAM_H
