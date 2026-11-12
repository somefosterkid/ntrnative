// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/shortcut_util.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_SHORTCUT_UTIL_H
#define BWPORT_SYSTEM_SHORTCUT_UTIL_H

#include "types.h"

// Black 2's, not checked against Black
// Where the shortcut menu's list was left, which GameData_Create clears
typedef struct {
    u16 unk0;
    u16 unk2;
} ShortcutMenuPos;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ShortcutUtil_GetItemShortcut
// The shortcut of an item that registers to nothing: Black's 0x19
#define SHORTCUT_NONE 0x19
// The shortcut that registers the key item to Y, or SHORTCUT_NONE
u32 func_0202e448(u16 item);

#endif // BWPORT_SYSTEM_SHORTCUT_UTIL_H
