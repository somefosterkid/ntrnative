// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/shortcut.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SAVE_SHORTCUT_H
#define BWPORT_SAVE_SHORTCUT_H

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ShortcutSave *SaveControl_GetShortcutSave(SaveControl *save);
u32 ShortcutSave_GetShortcutCount(ShortcutSave *shortcutSave);
// The shortcut in a slot
u8 func_0200c464(ShortcutSave *shortcutSave, u8 index);
// Moves a shortcut to a slot, or to the first free one when index is SHORTCUT_NONE. Returns the slot
u8 ShortcutSave_MoveItemToIndex(ShortcutSave *shortcutSave, u32 shortcut, u8 index);

// How many of the slots are used (Black's)
#define SHORTCUT_SAVE_MAX 0x19

// The registered shortcuts, without gaps, SHORTCUT_NONE when a slot is empty (Black's size)
struct ShortcutSave {
    u8 ids[0x1c];
};

#endif // BWPORT_SAVE_SHORTCUT_H
