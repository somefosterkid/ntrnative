// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov146/event_encounter_cutin.c, ported by tools/port_file.py.
#include "types.h"
#include "field/encounter_effect.h"
#include "field/event_encounter_cutin.h"
#include "field/field.h"
#include "gfl/fade.h"
#include "system/game_event.h"
#include "system/game_system.h"

struct EncounterCutinWork {
    s32 state;
    u32 selection;
    u32 param;
};

// The create functions of the encounter effects that show a cut-in, by the cut-in they show

GameEvent *func_ov073_021f5260(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 0, param);
}

GameEvent *func_ov073_021f526c(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 1, param);
}

GameEvent *func_ov073_021f5278(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 2, param);
}

GameEvent *func_ov073_021f5284(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 3, param);
}

GameEvent *func_ov073_021f5290(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 4, param);
}

GameEvent *func_ov073_021f529c(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 5, param);
}

GameEvent *func_ov073_021f52a8(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 6, param);
}

GameEvent *func_ov073_021f52b4(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 7, param);
}

GameEvent *func_ov073_021f52c0(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 8, param);
}

GameEvent *func_ov073_021f52cc(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 9, param);
}

GameEvent *func_ov073_021f52d8(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 10, param);
}

GameEvent *func_ov073_021f52e4(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 11, param);
}

GameEvent *func_ov073_021f52f0(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 12, param);
}

GameEvent *func_ov073_021f52fc(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 13, param);
}

GameEvent *func_ov073_021f5308(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 14, param);
}

GameEvent *func_ov073_021f5314(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 15, param);
}

GameEvent *func_ov073_021f5320(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 16, param);
}

GameEvent *func_ov073_021f532c(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 17, param);
}

GameEvent *func_ov073_021f5338(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 18, param);
}

GameEvent *func_ov073_021f5344(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 19, param);
}

GameEvent *func_ov073_021f5350(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 20, param);
}

GameEvent *func_ov073_021f535c(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 21, param);
}

GameEvent *func_ov073_021f5368(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 22, param);
}

GameEvent *func_ov073_021f5374(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 23, param);
}

GameEvent *func_ov073_021f5380(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 24, param);
}

GameEvent *func_ov073_021f538c(GameSystem *gsys, Field *field, u32 param) {
    return EventEncountEffectCutin_Create(gsys, field, 25, param);
}

GameEvent *EventEncountEffectCutin_Create(GameSystem *gsys, Field *field, u32 selection, u32 param) {
    EncounterCutinWork *work;

    work = EncEff_AllocWorkArea(Field_GetEncEff(GSYS_GetField(gsys)), 0xc, 0x50);
    work->state = 0;
    work->selection = selection;
    work->param = param;
    return GameEvent_Create(gsys, NULL, EventEncountEffectCutin_Callback, 0);
}

GameEvent *func_ov021_021c1dcc(GameSystem *gsys, void *ci, u32 selection, u32 param);
GameEventReturnCode EventEncountEffectCutin_Callback(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    EncounterCutinWork *work;
    GameEvent *next;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    work = EncEff_GetWorkArea(Field_GetEncEff(field));

    switch (*state) {
    case 0:
        GFL_FadeSet(4, 0, 0x10, 0);
        (*state)++;
    case 1:
        if (!GFL_FadeIsRunning()) {
            GFL_FadeSet(4, 0x10, 0, 0);
            (*state)++;
        }
        break;
    case 2:
        if (!GFL_FadeIsRunning()) {
            work->state++;
            if (work->state < 2) {
                GFL_FadeSet(4, 0, 0x10, 0);
                *state = 1;
            } else {
                next = func_ov021_021c1dcc(gsys, Field_Get3DCi(field), work->selection, work->param);
                if (next != NULL) {
                    GameEvent_ChainNext(event, next);
                } else {
                    next = GameEvent_Create(gsys, event, EventEncountEffectCutinFallback_Callback, 0);
                    GameEvent_ChainNext(event, next);
                }
                (*state)++;
            }
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventEncountEffectCutinFallback_Callback(GameEvent *event, u32 *state, void *data) {
    GSYS_GetField(GameEvent_GetGameSystem(event));
    switch (*state) {
    case 0:
        GFL_FadeSet(3, 0, 0x10, 3);
        (*state)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}
