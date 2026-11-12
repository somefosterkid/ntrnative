// Black's code at ov018 0x0217cdbc-0x0217cdd0. Placed by tools/gap_files.py from 1 pokebw2; height_ex.c is ov021's file
// (gap_setup.py adds _ov018). (height_ex_ov018.c: another module has a file of the name)

#include "types.h"
#include "field/event_mapchange.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"

struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};

#include "field/field_script.h"
u32 func_ov018_0217cdd0(GameEvent *param_1, s32 *param_2);

GameEvent *func_ov018_0217cdbc(GameSystem *gsys) {
    GameEvent *pGVar1;

    pGVar1 = GameEvent_Create(gsys, (GameEvent *)0x0, (void *)func_ov018_0217cdd0, 0);
    return pGVar1;
}

u32 func_ov018_0217cdd0(GameEvent *event, s32 *state) {
    switch (*state) {
    case 0:
        EventScriptCall_Start(event, 0x279e, NULL, 0, HEAPID_FIELDMAP);
        (*state)++;
        break;
    case 1:
        return TRUE;
    }
    return FALSE;
}
