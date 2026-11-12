#ifndef POKEBW2_SYSTEM_TIME_ICON_H
#define POKEBW2_SYSTEM_TIME_ICON_H

#include "types.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"

typedef struct WaitIcon WaitIcon;

// The hourglass (time_icon.c) that turns in the bottom right corner of a window while the game is busy. It clears its
// corner with clearColor when it goes, and shows each of its frames for interval frames. Our names

// Created with its bitmap only, and shown and stepped by a task of the manager once started
WaitIcon *WaitIcon_Alloc(HeapID heapId);
void WaitIcon_Start(WaitIcon *icon, TCBManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval);
void WaitIcon_Free(WaitIcon *icon);
// Created shown, stepped by a task of the manager, or by its owner through WaitIcon_Main if the manager is NULL
WaitIcon *WaitIcon_Create(TCBManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval, HeapID heapId);
void WaitIcon_Main(WaitIcon *icon);
// Created shown, as the data of a task of the manager
WaitIcon *WaitIcon_CreateTCBEx(TCBExManager *tcbManager, BmpWin *window, u8 clearColor, u8 interval, HeapID heapId);

#endif // POKEBW2_SYSTEM_TIME_ICON_H
