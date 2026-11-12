// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2_bmp.h: the lines in use so far.
#ifndef BWPORT_APP_BOX2_BMP_H
#define BWPORT_APP_BOX2_BMP_H

#include "types.h"
#include "struct_decls.h"

void func_ov170_021ebb1c(Box2SysWork *syswk, u32 index);
void func_ov170_021ebb58(Box2SysWork *syswk, u32 index);
void func_ov170_021ebc28(Box2SysWork *syswk, u32 type, u32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Makes the frames of the menu's buttons and of the buttons on the lower screen
void func_ov170_021eb744(Box2AppWork *app);
void func_ov170_021eb79c(Box2SysWork *syswk);
void func_ov170_021eb7c8(Box2SysWork *syswk);
void func_ov170_021eb95c(Box2SysWork *syswk);
void func_ov170_021ebb28(Box2SysWork *syswk, u32 index);
void func_ov170_021ebb34(Box2SysWork *syswk, u32 item, u32 index);
void func_ov170_021ebb64(Box2SysWork *syswk, u32 item, u32 index);
void func_ov170_021ebbbc(Box2SysWork *syswk, u32 index);
void func_ov170_021ebbc8(Box2SysWork *syswk, u32 type, u32 index);
// Shows how many Pokémon a box holds on a text object
void func_ov170_021eb874(Box2SysWork *syswk, u32 tray, u32 index);
void func_ov170_021eb85c(Box2SysWork *syswk, u32 mv);
// Sends the characters of the printed windows to VRAM
void func_ov170_021eab04(Box2AppWork *app);
// Shows a box's name on a text object
void func_ov170_021eb7e8(Box2SysWork *syswk, u32 tray, u32 index);
// Shows a Pokémon's data on the upper screen, or clears it
void func_ov170_021eb2a4(Box2SysWork *syswk, Box2PokeInfo *info);
void func_ov170_021eb324(Box2AppWork *app);
void func_ov170_021ebcb4(Box2SysWork *syswk, u32 type, u32 index);
// Opens a menu of items
void func_ov170_021eb988(Box2SysWork *syswk, const u16 *data, u32 count);
void func_ov170_021eba5c(Box2AppWork *app, u32 index);
void func_ov170_021eba70(Box2AppWork *app, u32 index);
void func_ov170_021ebc58(Box2SysWork *syswk, u32 a, u32 type, u32 index);
void func_ov170_021ebadc(Box2SysWork *syswk, u32 index);
void func_ov170_021ebd1c(Box2SysWork *syswk, u32 type, u32 index);
void func_ov170_021ebd50(Box2SysWork *syswk);
void func_ov170_021ebaf8(Box2SysWork *syswk, u32 itemId, u32 a);
void func_ov170_021ebd78(Box2SysWork *syswk, u32 a);

#endif // BWPORT_APP_BOX2_BMP_H
