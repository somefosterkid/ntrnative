// Black's code at main 0x0200c404-0x0200c520. The name is pokebw2's file between trainer_game_info_savedata.c and
// musical_dist_save.c (10 functions there, 8 here).

#include "types.h"
#include "save/save_control.h"
#include "save/shortcut.h"
#include "struct_decls.h"
#include "nitro/math.h"
#include "system/shortcut_util.h"

void func_0200c4e8(ShortcutSave *shortcutSave);
ShortcutSave *func_0200c40c(SaveControl *save);


ShortcutSave *SaveControl_GetShortcutSave(SaveControl *save) {
    return func_0200c40c(save);
}

ShortcutSave *func_0200c40c(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, 0x36);
}

// pokebw2's ShortcutSave_SetKeyItemRegistration
void func_0200c418(ShortcutSave *shortcutSave, u32 shortcut, BOOL registered) {
    int i;

    if (registered) {
        for (i = 0; i < SHORTCUT_SAVE_MAX; i++) {
            if (shortcutSave->ids[i] == shortcut) {
                return;
            }
            if (shortcutSave->ids[i] == SHORTCUT_NONE) {
                break;
            }
        }
        shortcutSave->ids[i] = shortcut;
    } else {
        for (i = 0; i < SHORTCUT_SAVE_MAX; i++) {
            if (shortcutSave->ids[i] == shortcut) {
                shortcutSave->ids[i] = SHORTCUT_NONE;
            }
        }
    }
    func_0200c4e8(shortcutSave);
}

// pokebw2's ShortcutSave_IsRegistered
BOOL func_0200c44c(ShortcutSave *shortcutSave, u32 shortcut) {
    int i;

    for (i = 0; i < SHORTCUT_SAVE_MAX; i++) {
        if (shortcutSave->ids[i] == shortcut) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 func_0200c464(ShortcutSave *shortcutSave, u8 index) {
    return shortcutSave->ids[index];
}

u32 ShortcutSave_GetShortcutCount(ShortcutSave *shortcutSave) {
    s32 iVar1;
    u32 uVar2;

    uVar2 = 0;
    iVar1 = 0;
    do {
        if (shortcutSave->ids[iVar1] != 0x19) {
            uVar2 = uVar2 + 1;
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0x19);
    return uVar2 & 0xffff;
}

// Moves a shortcut to a slot, or to the first free one when index is 0xff. Returns the slot
u8 ShortcutSave_MoveItemToIndex(ShortcutSave *shortcutSave, u32 shortcut, u8 index) {
    s16 i;
    s16 old;

    for (old = 0; old < SHORTCUT_SAVE_MAX; old++) {
        if (shortcutSave->ids[old] == shortcut) {
            break;
        }
    }
    func_0200c418(shortcutSave, shortcut, FALSE);
    if (index == 0xff) {
        for (index = 0; index < SHORTCUT_SAVE_MAX; index++) {
            if (shortcutSave->ids[index] == SHORTCUT_NONE) {
                shortcutSave->ids[index] = shortcut;
                break;
            }
        }
    } else {
        if (old < index) {
            index--;
        }
        for (i = SHORTCUT_SAVE_MAX - 1; i >= index; i--) {
            shortcutSave->ids[i] = shortcutSave->ids[i - 1];
        }
        shortcutSave->ids[index] = shortcut;
    }
    return index;
}


// pokebw2's ShortcutSave_Condense: moves each empty slot one on, so that the shortcuts close up
void func_0200c4e8(ShortcutSave *shortcutSave) {
    u16 i;

    for (i = 0; i < SHORTCUT_SAVE_MAX; i++) {
        u16 next = MATH_CLAMP(i + 1, 1, SHORTCUT_SAVE_MAX - 1);
        u8 id = shortcutSave->ids[i];

        if (id == SHORTCUT_NONE) {
            shortcutSave->ids[i] = shortcutSave->ids[next];
            shortcutSave->ids[next] = id;
        }
    }
}

