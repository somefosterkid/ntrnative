// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2_ui.h: the lines in use so far.
#ifndef BWPORT_APP_BOX2_UI_H
#define BWPORT_APP_BOX2_UI_H

#include "types.h"
#include "struct_decls.h"

// The position of the box list at a touch, or BOX2_GET_NONE
u32 func_ov170_021ef224(u32 x, u32 y);
u32 func_ov170_021ef1b4(void);
u32 func_ov170_021ef264(void);
// The position at a touch
u32 func_ov170_021ef180(u32 x, u32 y);

#define CURSORMOVE_NONE 0xffffffff

// Black 2's, not checked against Black
#define CURSORMOVE_CANCEL 0xfffffffe
#define CURSORMOVE_CURSOR_MOVE 0xfffffffd
#define CURSORMOVE_CURSOR_ON 0xfffffffc
#define CURSORMOVE_SCROLL_R 0xfffffffb
#define CURSORMOVE_SCROLL_L 0xfffffffa
#define CURSORMOVE_UNK_7 0xfffffff9
#define CURSORMOVE_UNK_8 0xfffffff8

u32 func_ov170_021ef170(void);
void func_ov170_021ee544(Box2SysWork *syswk, u32 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov170_021ee2c4(Box2SysWork *syswk);
void func_ov170_021ee364(Box2SysWork *syswk, u32 a1, u32 pos);
void func_ov170_021ee3cc(Box2SysWork *syswk);
void func_ov170_021ee3e4(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee474(Box2SysWork *syswk);
void func_ov170_021ee520(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee57c(Box2SysWork *syswk);
u32 func_ov170_021ee65c(Box2SysWork *syswk);
void func_ov170_021ee754(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee850(Box2SysWork *syswk);
u32 func_ov170_021ee8a8(Box2SysWork *syswk);
u32 func_ov170_021ee96c(Box2SysWork *syswk);
u32 func_ov170_021eea48(Box2SysWork *syswk);
u32 func_ov170_021eeb40(Box2SysWork *syswk);
u32 func_ov170_021eece4(Box2SysWork *syswk);
u32 func_ov170_021eede0(Box2SysWork *syswk);
u32 func_ov170_021eeea4(Box2SysWork *syswk);
void func_ov170_021eef74(Box2AppWork *app, u32 pos, u32 curPos);
s32 func_ov170_021ef1c4(void);

#endif // BWPORT_APP_BOX2_UI_H
