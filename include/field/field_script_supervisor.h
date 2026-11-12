// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_script_supervisor.h: the
// prototypes of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_SCRIPT_SUPERVISOR_H
#define BWPORT_FIELD_FIELD_SCRIPT_SUPERVISOR_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

BOOL FieldScriptSupervisor_Update(FieldScriptSupervisor *supervisor);
int FieldScriptSupervisor_AddVM(FieldScriptSupervisor *supervisor, VM *vm);
VM *FieldScriptSupervisor_GetVM(FieldScriptSupervisor *supervisor, int index);
void FieldScriptSupervisor_CallPostFunc(FieldScriptSupervisor *supervisor, GameEvent *event);
BOOL FieldScriptSupervisor_HasPostFunc(FieldScriptSupervisor *supervisor);

// Black 2's; twinned code confirms the offsets of heapId, vmCount, vms, postFunc and postArg in Black
// Layout and names reconstructed from the field script code and swan.
struct FieldScriptSupervisor {
    HeapID heapId;
    u8 vmCount;
    u8 padding;
    VM *vms[3];
    void (*postFunc)(GameEvent *, void *);
    void *postArg;
};

void FieldScriptSupervisor_SetPostEvent(FieldScriptSupervisor *supervisor, GameEvent *event);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldScriptSupervisor *FieldScriptSupervisor_Create(HeapID heapId);
void FieldScriptSupervisor_Free(FieldScriptSupervisor *supervisor);
VM *FieldScript_CreateVM(HeapID heapId, ScriptWork *work, u16 zoneId, u16 scriptId, u32 featureLevel);
void FieldScript_FreeVM(VM *vm);
void FieldScriptTerminator_ReplaceEvent(GameEvent *event, void *arg);
u16 FieldScript_GetZoneIDFromGSys(GameSystem *gsys);

#endif // BWPORT_FIELD_FIELD_SCRIPT_SUPERVISOR_H
