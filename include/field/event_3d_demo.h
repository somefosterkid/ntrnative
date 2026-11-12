// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_3d_demo.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_3D_DEMO_H
#define BWPORT_FIELD_EVENT_3D_DEMO_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "app/demo3d.h"
#include "system/game_event.h"

void CreateSeqLoadParam(Demo3DParam *out, GameSystem *gsys, u32 demoId, u8 param, u32 arg4, u8 hour, u8 minute,
                        u8 season);

// Black 2's without its demoId and unk26 after savedState (Black allocates 0x24); matching code confirms the offsets
// of gameSystem, gameCommSys, param and savedState in Black
struct Event3DDemoWork {
    GameSystem *gameSystem;
    GameCommSys *gameCommSys;
    Demo3DParam param;
    // What func_02012144 returned before the demo
    u32 savedState;
}; // size: 0x24

void SetupPlaySequenceEventGameTime(Demo3DParam *out, GameSystem *gsys, u8 demoId, u8 param);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *Event3DDemo_Create(GameSystem *gsys, GameEvent *parent, u32 demoId, u32 param, u32 realTime);
void SetupPlaySequenceEventRealTime(Demo3DParam *out, GameSystem *gsys, u8 demoId, u8 param);
GameEventReturnCode Event3DDemo_Callback(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_EVENT_3D_DEMO_H
