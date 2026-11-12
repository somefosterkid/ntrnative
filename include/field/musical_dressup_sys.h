// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/musical_dressup_sys.h.
#ifndef BWPORT_FIELD_MUSICAL_DRESSUP_SYS_H
#define BWPORT_FIELD_MUSICAL_DRESSUP_SYS_H

// Overlay 10's musical_dressup_sys.c: the proc that runs overlay 133's dressing room

#include "types.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "struct_decls.h"

typedef struct {
    // Overlay 132's communication work, or NULL
    void *comm;
    MusicalPoke *poke;
    MusicalSave *save;
} MusicalDressUpParam;

MusicalDressUpParam *func_ov010_02156cac(HeapID heapId, MusicalPoke *poke, SaveControl *save);
void func_ov010_02156cd8(MusicalDressUpParam *param);

extern GameProcFunctions data_ov010_02172724;

// Overlay 133, the dressing room, which gets a copy of the parameter (pokebw2's func_ov208_021998c0,
// func_ov208_02199a54 and func_ov208_02199b7c)
void *func_ov133_021b6100(MusicalDressUpParam *param, HeapID heapId);
void func_ov133_021b6294(struct DupFittingWork *work);
u32 func_ov133_021b63bc(struct DupFittingWork *work);

#endif // BWPORT_FIELD_MUSICAL_DRESSUP_SYS_H
