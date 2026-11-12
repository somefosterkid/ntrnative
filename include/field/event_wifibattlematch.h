// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_wifibattlematch.h. pokebw2's
// EventWifiBattleMatch_Create and EventWifiBattleMatch_CreateFromArgs are its own names.
#ifndef BWPORT_FIELD_EVENT_WIFIBATTLEMATCH_H
#define BWPORT_FIELD_EVENT_WIFIBATTLEMATCH_H

// Wi-Fi battles, started by the NetConnectWiFiBattle script command

#include "types.h"
#include "struct_decls.h"

typedef struct {
    Field *field;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} EventWifiBattleMatchArgs;

GameEvent *func_ov002_021543e8(GameSystem *gsys, Field *field, u32 unk10, u32 unk14, u32 unk18);
GameEvent *func_ov002_02154438(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_EVENT_WIFIBATTLEMATCH_H
