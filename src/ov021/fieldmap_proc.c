// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fieldmap_proc.c.
// The field map's process, which makes the field's heaps and its Field (fieldmap.c) and runs the field's routines.
// The ROM doesn't name the file; the name is pokebw2's guess. Its FIELDMAP_PROC_FUNCS lies ahead of fieldmap.c's
// smallest table, and no layout of one file gives that.
#include "types.h"
#include "field/field.h"
#include "field/field_internal.h"
#include "field/player_state.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "system/game_comm.h"
#include "system/game_system.h"
#include "work_area.h"

// Heap 0x89's memory, twice the size on a PC target (work_area.h), as the heap system makes the child heaps in it
u8 g_FieldHeapMemory[WORK_AREA_SIZE(0x10000)];

const GameProcFunctions FIELDMAP_PROC_FUNCS = { FieldmapProc_Init, FieldmapProc_Update, FieldmapProc_End };

BOOL FieldmapProc_Init(GameProc *proc, u32 *seq, void *param, void *work) {
    GameSystem *gsys = param;

    switch (*seq) {
    case 0: {
        Field **procWork;
        u32 heapSize = GetFieldmapZoneHeapSize(PlayerState_GetZoneID(GSYS_GetPlayerState(gsys))) - 0xd000;

        GFL_HeapCreateChild(HEAPID_USER, HEAPID_FIELDMAP, heapSize);
        GFL_HeapCreateChild(HEAPID_USER, 0x70, 0xd000);
        GFL_HeapCreateChild(HEAPID_FIELDMAP, 0x50, 0xc000);
        GFL_HeapCreateRoot(g_FieldHeapMemory, sizeof(g_FieldHeapMemory), 0x89);
        GFL_HeapCreateChild(0x89, 0x92, 0x6400);
        GFL_HeapCreateChild(0x89, 0x93, 0x7400);
        GFL_HeapCreateChild(0x89, 0x96, 0x500);
        procWork = GFL_ProcInitSubsystem(proc, sizeof(Field *), HEAPID_FIELDMAP);
        *procWork = Field_Create(gsys, HEAPID_FIELDMAP);
        GSYS_SetField(gsys, *procWork);
        (*seq)++;
        break;
    }
    case 1:
        if (!func_0201214c(gsys)) {
            GameCommSys *comm = GSYS_GetGameCommSystem(gsys);

            switch (GameCommSys_BootCheck(comm)) {
            case 1:
            case 2:
                func_02026400(comm);
                break;
            }
        }
        return TRUE;
    }
    return FALSE;
}

BOOL FieldmapProc_Update(GameProc *proc, u32 *seq, void *param, void *work) {
    if (Field_CallRoutines(param, *(Field **)work) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL FieldmapProc_End(GameProc *proc, u32 *seq, void *param, void *work) {
    GameSystem *gsys = param;

    Field_Free(*(Field **)work);
    GSYS_SetField(gsys, NULL);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x96);
    GFL_HeapDelete(0x92);
    GFL_HeapDelete(0x93);
    GFL_HeapDelete(0x89);
    GFL_HeapDelete(0x50);
    GFL_HeapDelete(0x70);
    GFL_HeapDelete(HEAPID_FIELDMAP);
    return TRUE;
}
