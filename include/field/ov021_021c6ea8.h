#ifndef BWPORT_FIELD_OV021_021C6EA8_H
#define BWPORT_FIELD_OV021_021C6EA8_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"
#include "field/field_task.h"

void FieldTaskManager_UpdateParentControlTransfer(FieldTaskManager *mgr, u8 index);
void FieldTaskManager_UpdateEndTasks(FieldTaskManager *mgr, u8 index);
// Moves the camera's target from one position to another over duration frames
FieldTask *FieldCameraMoveTaskTarget_Create(Field *field, u16 duration, const VecFx32 *from, const VecFx32 *to);
// pokebw2 declares these in field_task.h. They wait here: ov021_021bf58c.c, which includes field_task.h, still
// declares its own copies (u32 * returns, integer arguments)
// The player's spin slowing down to face dir
FieldTask *FieldActorSpinTask_CreatePlayerDeaccel(Field *field, u32 duration, u32 turns, u32 dir);
// pokebw2's FieldPlayerMoveTask_Create: moves the player to pos over duration frames
FieldTask *func_ov021_021c754c(Field *field, u32 duration, VecFx32 *pos);
// Waits for wait frames
FieldTask *FieldWaitTask_Create(Field *field, u16 wait);
FieldTask *FieldFadeTask_Create(Field *field, u32 mode, u32 from, u32 to, u32 speed);
// Runs func_ov021_021add7c on the player's actor every interval frames for duration frames (actor goes unused)
FieldTask *FieldActorEffectTask_Create(Field *field, FieldActor *actor, u16 duration, u16 interval);

#endif // BWPORT_FIELD_OV021_021C6EA8_H
