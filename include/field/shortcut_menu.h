// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/shortcut_menu.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SHORTCUT_MENU_H
#define BWPORT_FIELD_SHORTCUT_MENU_H

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's; twinned code confirms the offsets of gameSystem, field, app, done and state in Black
struct ShortcutMenuWork {
    GameEvent *event;
    GameSystem *gameSystem;
    Field *field;
    void *app;
    FieldAppCallInput *input;
    u16 code;
    u32 done;
    u32 state;
};

BOOL IsExistAnyYShortcut(GameSystem *gsys);
BOOL CheckAnyRibbon(ShortcutMenuWork *work);
void func_ov010_021605c0(u32 arg0, ShortcutMenuWork *work);
BOOL func_ov010_0216060c(ShortcutMenuWork *work);
BOOL func_ov010_021606d0(FieldAppCallInput *input, void *arg);

void func_ov021_021c51a0(void *app);
void func_ov021_021c5224(void *app);
BOOL func_ov021_021c5334(void *app);
void func_ov021_021c536c(void *app);
BOOL func_ov021_021c5380(void *app);
u32 func_ov021_021c5394(void *app, u32 *item);
GameEvent *func_ov021_021c5a28(GameSystem *gsys, Field *field, void *param, BOOL transition);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 ShortcutMenu_SetKeyItemID(FieldAppCallInput *input, u32 item);
BOOL ShortcutMenu_GetActionFromKeyItem(s32 item, u32 *action, u32 *invalid);
GameEvent *CallYButtonShortcutMenu(GameSystem *gsys, Field *field, u16 code);
GameEventReturnCode EventShortcutChoicePopup_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventShortcutCallDirect_Callback(GameEvent *event, u32 *state, void *data);
void func_ov021_02190344(FieldMsgBG *msgBG);
void func_ov021_02190398(FieldMsgBG *msgBG);
BOOL func_ov010_02160668(FieldAppCallInput *input, void *arg);
void *func_ov021_021c4f7c(GameData *gameData, u32 mode, u16 *listPos, HeapID heapId, HeapID allocHeapId);
void func_ov021_021c5348(void *app);

#endif // BWPORT_FIELD_SHORTCUT_MENU_H
