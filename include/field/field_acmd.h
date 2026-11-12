// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_acmd.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_ACMD_H
#define BWPORT_FIELD_FIELD_ACMD_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// An action command: the command in the low 16 bits and how many times to run it in the high 16. A list of them ends
// with ACMD_END
#define ACMD(code, count) (((count) << 16) | (code))
#define ACMD_END 0xfe

FieldAcmdTCB *FieldAcmdTCB_Create(FieldActor *actor, const u32 *action);
BOOL FieldAcmdTCB_CheckEnded(FieldAcmdTCB *task);
void FieldAcmdTCB_Remove(FieldAcmdTCB *task);

extern const u32 ACMD_QUEUE_WALK_N_8F[2];
extern const u32 ACMD_QUEUE_WALK_S_8F[2];
extern const u32 ACMD_QUEUE_WALK_W_8F[2];
extern const u32 ACMD_QUEUE_WALK_E_8F[2];

#endif // BWPORT_FIELD_FIELD_ACMD_H
