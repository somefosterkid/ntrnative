// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_status.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_STATUS_H
#define BWPORT_FIELD_FIELD_STATUS_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// pokebw2's func_02019314
BOOL func_0201444c(FieldStatus *status);

FieldStatus *FieldStatus_Create(HeapID heapId);

// Black 2's; matching code confirms the offsets of inLinkedWorld, newLoad, isContinue, flashUsed, flashPerms, busyFlag
// and reservedScript, and its size in Black
struct FieldStatus {
    BOOL inLinkedWorld;
    u8 newLoad;
    u8 isContinue;
    u8 flashUsed;
    u16 flashPerms;
    u8 unk0A[7];
    u8 busyFlag;
    // The script to run on the next map, 0 for none
    u16 reservedScript;
}; // 0x14

// Returns the flag it replaces
u32 FieldStatus_SetBusyFlag(FieldStatus *status, u32 flag);
void FieldStatus_SetContinueFlag(FieldStatus *status, BOOL flag);

#define FLD_FLASH_ALLOW 0x1
#define FLD_FLASH_ACTIVE 0x2

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL FieldStatus_CheckFlashUsed(FieldStatus *status);
u16 FieldStatus_GetFlashPerms(FieldStatus *status);
void FieldStatus_ReserveScript(FieldStatus *status, u16 scriptId);
u32 FieldStatus_GetBusyFlag(FieldStatus *status);
void FieldStatus_SetFlashPerms(FieldStatus *status, u32 flags);
void FieldStatus_SetFlashUsed(FieldStatus *status, BOOL value);
void FieldStatus_Free(FieldStatus *status);
BOOL FieldStatus_CheckContinueFlag(FieldStatus *status);
u16 FieldStatus_GetReserveScript(FieldStatus *status);
void FieldStatus_SetNewLoadFlag(FieldStatus *status, BOOL flag);
u8 FieldStatus_GetNewLoadFlag(FieldStatus *status);
// pokebw2's FieldStatus_SetInLinkedWorld
void func_02014450(FieldStatus *status, BOOL inLinkedWorld);

#endif // BWPORT_FIELD_FIELD_STATUS_H
