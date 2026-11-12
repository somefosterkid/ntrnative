// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_dendou_machine.h.
#ifndef BWPORT_FIELD_EVENT_DENDOU_MACHINE_H
#define BWPORT_FIELD_EVENT_DENDOU_MACHINE_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct EventDendouMachineData {
    u16 heapId;
    u16 unk02;
    Field *field;
    VecFx32 basePosition;
    u8 ballCount;
    u8 spawnedCount;
    u16 frameCounter;
    FieldPropSystem *propSystem;
    FieldPropHandle *ballHandles[6];
    FieldChunkPropHolder *centerProp;
    FieldPropHandle *centerHandle;
};

GameEvent *EventDendouMachine_Create(GameSystem *gsys, GameEvent *parent);
GameEventReturnCode EventDendouMachine_Callback(GameEvent *event, u32 *state, void *data);
void EventDendouMachine_Init(EventDendouMachineData *work, GameSystem *gsys);
void EventDendouMachine_End(EventDendouMachineData *work);
void EventDendouMachine_SpawnMonsBall(EventDendouMachineData *work);
void EventDendouMachine_StartAnimations(EventDendouMachineData *work);
BOOL EventDendouMachine_IsAnimationDone(EventDendouMachineData *work);
int countNonEggsInParty(GameSystem *gsys);

#endif // BWPORT_FIELD_EVENT_DENDOU_MACHINE_H
