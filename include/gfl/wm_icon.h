// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/wm_icon.h.
#ifndef BWPORT_GFL_WM_ICON_H
#define BWPORT_GFL_WM_ICON_H

#include "types.h"
#include "gfl/heap.h"

// The wireless signal icon in a corner of a screen, drawn with the first two OBJ of OAM

// Which screen the icon is on
enum {
    WM_ICON_SCREEN_TOP = 1,
    WM_ICON_SCREEN_BOTTOM,
};

// Shows the icon, or shows it again with new graphics when it is already shown
void func_02037010(u16 x, u16 y, u32 type, u32 heapId);
void func_020370c8(void);
// The signal strength, 0 to 3
void func_020370f4(int level);
void func_02037118(BOOL top, HeapID heapId);
// Writes the icon to OAM, each frame
void func_02037154(void);
void func_02037174(void);
void func_02037190(void);
// Puts the icon at x and y in place of where func_02037010 says, unless x is 0
void func_020371ac(u16 x, u16 y);
// The 2D engine that draws the icon, 1 for the main engine and 2 for the sub engine
u32 func_02037200(void);

// The game's archive of the icon's graphics, and its files: two sets of characters and the palette
typedef struct {
    u32 chars[2];
    u32 pltt;
} WmIconFiles;

u32 getWifiIconNarcIdxNum(void);
void func_0200f16c(WmIconFiles *files);

#endif // BWPORT_GFL_WM_ICON_H
