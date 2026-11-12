// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/app_printsys_common.h, the lines in
// use so far.
#ifndef BWPORT_SYSTEM_APP_PRINTSYS_COMMON_H
#define BWPORT_SYSTEM_APP_PRINTSYS_COMMON_H

#include "types.h"
#include "struct_decls.h"
#include "system/printsys.h"

// The menus' handling of a message printed a character at a time: the keys and the touch screen, as `flags` allows,
// continue it after a pause and, pressed again after being released, speed it up

// What can continue or speed up the message
#define APP_PRINTSYS_COMMON_KEYS 0x2
#define APP_PRINTSYS_COMMON_TOUCH 0x4

typedef struct {
    u8 released;
    u32 flags;
} AppPrintsysCommon;

void func_02028470(AppPrintsysCommon *work, u32 flags); // pokebw2's AppPrintsysCommon_Init
// Returns whether the stream is done, or NULL (pokebw2's AppPrintsysCommon_Update)
BOOL func_02028484(AppPrintsysCommon *work, PrintStream *stream);

// The message can't be sped up
#define APP_PRINTSYS_COMMON_NO_SKIP 0x400

#endif // BWPORT_SYSTEM_APP_PRINTSYS_COMMON_H
