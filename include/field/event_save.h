// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_save.h, the lines in use so far,
// with Black's layout.
#ifndef BWPORT_FIELD_EVENT_SAVE_H
#define BWPORT_FIELD_EVENT_SAVE_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

typedef struct ReportWork ReportWork;

// Black's has no subscreen to go back to, which Black 2's keeps before report
struct EventSaveWork {
    SaveControl *save;
    GameSystem *gameSystem;
    Field *field;
    void *msgBgSys;
    u16 heapId;
    u16 pad12;
    u32 *result;
    ReportWork *report;
};

GameEvent *EventSave_Create(GameSystem *gsys, Field *field, u16 heapId, void *msgBgSys, u32 *result);
GameEventReturnCode EventSave_Callback(GameEvent *event, u32 *state, void *data);
// The free callback of the apps in the field's app list (pokebw2's func_ov012_0215c594)
void func_ov010_0216171c(void *param);
// Overlay 10's report_event.c: the report screen, which asks whether to save and saves. Returns 0 once saved, 1 when
// the player backed out, and 2 until then
u32 EventSave_Update(EventSaveWork *work, u32 *state);

#endif // BWPORT_FIELD_EVENT_SAVE_H
