// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/app_menu_common.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_APP_MENU_COMMON_H
#define BWPORT_SYSTEM_APP_MENU_COMMON_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The status icons, as AppMenuCommon_GetStatusIcon returns them
#define APP_STATUS_ICON_PARALYSIS 1

#define APP_STATUS_ICON_FREEZE 2
#define APP_STATUS_ICON_SLEEP 3
#define APP_STATUS_ICON_POISON 4
#define APP_STATUS_ICON_BURN 5
#define APP_STATUS_ICON_FAINTED 6
#define APP_STATUS_ICON_NONE 8

// The type and contest category icons: the palette, each icon's palette within it, the icons, cells and animations
u32 func_020275e8(void);
u32 func_020275f8(u8 type);
u32 func_020275fc(u32 mapping);
u32 func_02027600(u32 mapping);
// The touch bar's icons: their palette, characters, and cells and animations for an OBJ VRAM mapping mode
u32 func_02027614(void);
u32 func_02027618(void);
u32 func_0202761c(u32 mapping);
u32 func_02027620(u32 mapping);
u32 func_02027628(void);
u32 func_0202762c(void);
u32 func_02027630(void);
// pokebw2's AppMenuCommon_LoadBarScreen
// Loads the menu bar's screen into the bottom three rows of bg's screen buffer, with its characters starting at
// charBase and in palette `palette`
void func_02027634(ArcTool *arc, u8 bg, HeapID heapId, u32 charBase, u32 palette);
u32 func_02027694(void);
u32 func_02027698(void);
u32 func_0202769c(u32 mapping);
u32 func_020276a0(u32 mapping);
u32 func_020276a4(void);
u32 func_020276a8(void);
u32 func_020276ac(u32 mapping);
u32 func_020276b0(u32 mapping);
u32 func_020276b4(void);
u32 func_020276b8(void);
u32 func_020276bc(u32 mapping);
// pokebw2's AppMenuCommon_GetStatusIcon
// The status icon (APP_STATUS_ICON_*) of a party Pokémon: fainted, its status condition, or none
u32 func_020276c4(PartyPkm *pkm);
u32 func_02027710(u32 mapping);
u32 func_02027714(u32 mapping);
u32 func_02027718(u32 mapping);
u32 func_0202771c(u32 mapping);
// The files of a Poké Ball's icon: its palette and characters, where ball 0 counts as 4, then its cells and
// animations, the same for every ball
u32 func_02027720(u32 ball);
u32 func_0202772c(u32 ball);
u32 func_02027738(u32 ball, u32 mapping);
u32 func_02027740(u32 ball, u32 mapping);
u32 func_02027748(void);
u32 func_0202774c(u32 mapping);
u32 func_02027750(u32 mapping);
u32 func_02027754(u32 mapping);
u32 func_0202775c(u32 mapping);
u32 func_02027768(void);
u32 func_0202776c(u32 mapping);
u32 func_02027770(u32 mapping);
u32 func_02027774(u32 mapping);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 getUINarcIdx(void);
u8 func_020275ec(u8 type);
u8 func_02027604(u8 index);
u32 func_02027610(u8 index);
// The touch bar's BG: its palette, characters and screen
u32 func_02027624(void);
u32 func_020276c0(u32 mapping);
u32 func_02027758(void);
u32 func_02027760(u32 mapping);
u32 func_02027764(u32 mapping);
BOOL func_020275dc(void);
void func_020275e0(void);

#endif // BWPORT_SYSTEM_APP_MENU_COMMON_H
