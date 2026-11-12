#ifndef POKEBW2_SYSTEM_APP_PRINTSYS_COMMON_H
#define POKEBW2_SYSTEM_APP_PRINTSYS_COMMON_H

#include "types.h"
#include "system/printsys.h"

// The menus' handling of a message printed a character at a time (app_printsys_common.c, a guessed name): the keys
// and the touch screen, as `flags` allows, continue it after a pause and, pressed again after being released, speed it
// up

// What can continue or speed up the message
#define APP_PRINTSYS_COMMON_KEYS 0x2
#define APP_PRINTSYS_COMMON_TOUCH 0x4
// The message can't be sped up
#define APP_PRINTSYS_COMMON_NO_SKIP 0x400

typedef struct {
    u8 released;
    u32 flags;
} AppPrintsysCommon;

void AppPrintsysCommon_Init(AppPrintsysCommon *work, u32 flags);
// Returns whether the stream is done, or NULL
BOOL AppPrintsysCommon_Update(AppPrintsysCommon *work, PrintStream *stream);
// Whether a key or a touch, as flags allows, was pressed this frame; it also switches between touch and key mode.
// With no flags, always
BOOL AppPrintsysCommon_IsPressed(u32 flags);

#endif // POKEBW2_SYSTEM_APP_PRINTSYS_COMMON_H
