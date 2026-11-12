// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/app_printsys_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/app_printsys_common.h"
#include "system/printsys.h"

// The menus' handling of a message printed a character at a time. The file's name is a guess: the ROM has no string
// for it

static BOOL func_020284f4(u32 flags);
static BOOL func_02028528(u32 flags);
static BOOL func_0202855c(u32 flags);

// pokebw2's AppPrintsysCommon_Init
void func_02028470(AppPrintsysCommon *work, u32 flags) {
    MI_CpuFill8(work, 0, sizeof(AppPrintsysCommon));
    work->flags = flags;
}

// pokebw2's AppPrintsysCommon_Update
BOOL func_02028484(AppPrintsysCommon *work, PrintStream *stream) {
    if (stream != NULL) {
        switch (func_0201ce30(stream)) {
        case PRINT_STREAM_DONE:
            return TRUE;
        case PRINT_STREAM_PAUSED:
            if (func_020284f4(work->flags)) {
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
                func_0201ce38(stream);
            }
            break;
        case PRINT_STREAM_RUNNING:
            if ((work->flags & APP_PRINTSYS_COMMON_NO_SKIP) == 0) {
                // Only a press after everything was released speeds the message up
                if (work->released == FALSE) {
                    if (func_0202855c(work->flags)) {
                        work->released = TRUE;
                    }
                } else if (func_02028528(work->flags)) {
                    func_0201ce5c(stream, 0);
                }
            }
            break;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's AppPrintsysCommon_IsPressed
// Whether a key or a touch, as flags allows, was pressed this frame; it also switches between touch and key mode.
// With no flags, always
static BOOL func_020284f4(u32 flags) {
    BOOL pressed = FALSE;

    if (flags == 0) {
        pressed = TRUE;
    }
    if (flags & APP_PRINTSYS_COMMON_KEYS) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            pressed = TRUE;
        }
    }
    if (flags & APP_PRINTSYS_COMMON_TOUCH) {
        if (func_02035c60()) {
            pressed = TRUE;
        }
    }
    return pressed;
}


// pokebw2's AppPrintsysCommon_IsHeld
static BOOL func_02028528(u32 flags) {
    BOOL held = FALSE;

    if (flags == 0) {
        held = TRUE;
    }
    if (flags & APP_PRINTSYS_COMMON_KEYS) {
        if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            held = TRUE;
        }
    }
    if (flags & APP_PRINTSYS_COMMON_TOUCH) {
        if (func_02035c38()) {
            held = TRUE;
        }
    }
    return held;
}

// pokebw2's AppPrintsysCommon_IsReleased
static BOOL func_0202855c(u32 flags) {
    BOOL released = FALSE;

    if (flags == 0) {
        released = TRUE;
    }
    if (flags & APP_PRINTSYS_COMMON_KEYS) {
        if (GCTX_HIDGetHeldKeys() == 0) {
            released = TRUE;
        }
    }
    if (flags & APP_PRINTSYS_COMMON_TOUCH) {
        if (func_02035c38() == FALSE) {
            released = TRUE;
        }
    }
    return released;
}
