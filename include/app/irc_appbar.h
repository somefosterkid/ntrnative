// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/irc_appbar.h: the lines in use so far.
#ifndef BWPORT_APP_IRC_APPBAR_H
#define BWPORT_APP_IRC_APPBAR_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct IrcAppBar IrcAppBar;

// pokebw2's IrcAppBar_Free
void func_ov101_021b71c0(IrcAppBar *bar);
// pokebw2's IrcAppBar_Update
// Runs the button, which flashes when touched
void func_ov101_021b71e4(IrcAppBar *bar);
// pokebw2's IrcAppBar_SetActive
// Whether the button reacts, and goes back to waiting to be touched
void func_ov101_021b72a0(IrcAppBar *bar, BOOL active);

// pokebw2's IrcAppBar_SetVisible
void func_ov101_021b728c(IrcAppBar *bar, BOOL visible);

// The labels of the button
#define IRC_APPBAR_LABEL_CANCEL 0

#define IRC_APPBAR_LABEL_BACK 1

// pokebw2's IrcAppBar_Reset
void func_ov101_021b7264(IrcAppBar *bar);
// pokebw2's IrcAppBar_IsPressed
BOOL func_ov101_021b7254(IrcAppBar *bar);
// pokebw2's IrcAppBar_IsPressed
// pokebw2's IrcAppBar_Reset
// pokebw2's IrcAppBar_IsPressedOrDone
BOOL func_ov101_021b727c(IrcAppBar *bar);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's IrcAppBar_Create
IrcAppBar *func_ov101_021b70dc(u32 unused0, ClActUnit *unit, u8 bg, u8 palette, u8 objPalette, u32 unused5, Font *font,
                            PrintQueue *queue, HeapID heapId);
// pokebw2's IrcAppBar_GetResult
// What the button gave: 1 once it was touched and has flashed, otherwise -1
int func_ov101_021b7250(IrcAppBar *bar);
// pokebw2's IrcAppBar_SetLabel
void func_ov101_021b72a8(IrcAppBar *bar, int label);

#endif // BWPORT_APP_IRC_APPBAR_H
