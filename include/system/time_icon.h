// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/time_icon.h, the lines in use so far,
// with Black's names: pokebw2's WaitIcon_Create and WaitIcon_Free are its own guesses.
#ifndef BWPORT_SYSTEM_TIME_ICON_H
#define BWPORT_SYSTEM_TIME_ICON_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The hourglass drawn in a window's corner while the game saves
WaitIcon *func_0202ee30(TCBManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval, HeapID heapId);
// Created shown, as the data of a task of the manager (pokebw2's WaitIcon_CreateTCBEx)
WaitIcon *func_0202ee8c(TCBExManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval, HeapID heapId);
void func_0202f038(WaitIcon *icon);

// pokebw2's WaitIcon_Main
void func_0202f0b0(WaitIcon *icon);

// pokebw2's WaitIcon_Start
void func_0202ef98(WaitIcon *icon, TCBManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WaitIcon_Alloc
// Created with its bitmap only, and shown and stepped by a task of the manager once started
WaitIcon *func_0202ef60(HeapID heapId);

#endif // BWPORT_SYSTEM_TIME_ICON_H
