// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_sweet_scent.h.
#ifndef BWPORT_FIELD_EVENT_SWEET_SCENT_H
#define BWPORT_FIELD_EVENT_SWEET_SCENT_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct SweetScentEventData {
    u8 partySlot;
    u8 padding[3];
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    FieldPlayer *player;
};

struct SweetScentScreenWork {
    u8 bgId;
    u8 alpha;
    u8 step;
    u8 timer;
    void *displayControl;
};

struct SweetScentPalette {
    u16 first;
    u16 second;
};

GameEvent *EventSweetScent_Create(Field *field, GameSystem *gsys);
// Sweet Scent used by the party Pokémon in partySlot, which is shown using it unless partySlot is 0xff
GameEvent *func_ov021_021c6078(GameSystem *gsys, Field *field, u8 partySlot);

#endif // BWPORT_FIELD_EVENT_SWEET_SCENT_H
