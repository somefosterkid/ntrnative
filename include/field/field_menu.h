// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_menu.h, the lines in use so
// far. pokebw2's func_ov012_0215aa68, func_ov012_0215aa74, func_ov012_0215aa90 and func_ov012_0215aa94 are Black's
// func_ov010_0215fec8, func_ov010_0215fed4, func_ov010_0215fef0 and func_ov010_0215fef4.
#ifndef BWPORT_FIELD_FIELD_MENU_H
#define BWPORT_FIELD_FIELD_MENU_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "field/app_call.h"
#include "system/game_event.h"
#include "gfl/heap.h"

struct FieldMenuWork {
    u16 code;
    u16 pad02;
    GameEvent *event;
    GameSystem *gameSystem;
    Field *field;
    u32 unk10;
    u32 state;
    u32 screenId;
    // The app the menu opens, whose callbacks get the menu as their arg
    FieldAppCallInput appCall;
};

GameEvent *EventFieldMenu_Create(GameSystem *gsys, Field *field, u16 param);
GameEvent *EventFieldMenu_CreateUnionRoom(GameSystem *gsys, Field *field, u16 param);
GameEventReturnCode EventFieldMenu_Callback(GameEvent *event, u32 *state, void *data);

// The shortcut menu's file, after the field menu's
u32 func_ov010_0215fec8(u32 index);
BOOL func_ov010_0215fed4(FieldAppCallInput *input, void *arg);
BOOL func_ov010_0215fef0(FieldAppCallInput *input, void *arg);
BOOL func_ov010_0215fef4(FieldAppCallInput *input, void *arg);

// Overlay 21: the event that returns from the menu to the subscreen
GameEvent *EventFieldMenuReturn_Create(GameSystem *gsys, Field *field, u32 screenId);

// pokebw2's FieldMenu_Free
void func_ov021_021a84bc(FieldMenu *menu);
// pokebw2's FieldMenu_Update
void func_ov021_021a85b4(FieldMenu *menu);
// pokebw2's FieldMenu_UpdateScroll
// Scrolls the menu's BGs, once a frame
void func_ov021_021a8840(FieldMenu *menu);
// pokebw2's FieldMenu_LoadBar
// Loads the bar at the bottom of the touch screen
void func_ov021_021a8afc(HeapID heapId, BOOL hideBg4);
// pokebw2's FieldMenu_GetSelectedItem
// The item chosen, and moves the cursor to an item
u32 func_ov021_021a8e10(FieldMenu *menu);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's FieldMenu_Create
FieldMenu *func_ov021_021a836c(HeapID heapId, HeapID tmpHeapId, FieldSubscreen *subscreen, Field *field, BOOL open);
// pokebw2's FieldMenu_GetMenuType
// The kind of menu the zone has
u8 func_ov021_021a8b18(GameData *gameData, EventWork *eventWork, u32 zoneId);
// pokebw2's FieldMenu_SetCursorItem
void func_ov021_021a8e20(FieldMenu *menu, u32 itemId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
extern const u32 data_ov010_021712ac[8];

#endif // BWPORT_FIELD_FIELD_MENU_H
