// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2_obj.h: the lines in use so far.
#ifndef BWPORT_APP_BOX2_OBJ_H
#define BWPORT_APP_BOX2_OBJ_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"

// Black 2's, not checked against Black
// The actors, by ID: those of sActorData first, then these groups
#define BOX2_ACTOR_TYPE_ICON 32

// Black 2's, not checked against Black
#define BOX2_ACTOR_TRAY_ICON 49
#define BOX2_ACTOR_POKEICON 51
#define BOX2_ACTOR_ITEM_OUTLINE 121
#define BOX2_ACTOR_RANGE_DOT 129

void func_ov170_021ebdd8(Box2AppWork *app);
void func_ov170_021ec48c(Box2SysWork *syswk, u32 tray, u32 pos, u32 id);
// Where an icon at a position is
void func_ov170_021ec548(u32 pos, s16 *x, s16 *y, u32 mode);
void func_ov170_021ec94c(Box2PokeFreeWork *wk);
BOOL func_ov170_021ec994(Box2PokeFreeWork *wk);
BOOL func_ov170_021ec9d8(Box2PokeFreeWork *wk);
void func_ov170_021eca38(Box2PokeFreeWork *wk);
void func_ov170_021ebfcc(Box2AppWork *app, u32 id, u32 anim);
BOOL func_ov170_021ebfec(Box2AppWork *app, u32 idx);
void func_ov170_021ec7b8(Box2AppWork *app, u32 pos, u32 slot, u32 flag);
void func_ov170_021ebf34(Box2AppWork *app, u32 idx);
void func_ov170_021ebe24(Box2AppWork *app);
void func_ov170_021ebeac(Box2SysWork *syswk);
void func_ov170_021ebf50(Box2AppWork *app);
BOOL func_ov170_021ec01c(Box2AppWork *app, u32 idx);
void func_ov170_021ec030(Box2AppWork *app, u32 idx, u32 bgPriority);
void func_ov170_021ec048(Box2AppWork *app, u32 idx, u32 priority);
void *func_ov170_021ec338(Box2AppWork *app, BoxPkm *pkm, NNSG2dCharacterData **character);
void func_ov170_021ecb74(Box2SysWork *syswk, u32 pos);
void func_ov170_021ec08c(Box2AppWork *app, u32 id, s16 x, s16 y, u16 surface);
void func_ov170_021ed164(Box2AppWork *app, u32 pos, u32 mode);
void func_ov170_021ed198(Box2AppWork *app, u32 pos, u32 mode);
void func_ov170_021ed588(Box2SysWork *syswk);
void func_ov170_021ed888(Box2AppWork *app, BOOL visible);
void func_ov170_021ed904(Box2SysWork *syswk);
void func_ov170_021ed9b4(Box2AppWork *app);
void func_ov170_021edb1c(Box2SysWork *syswk);
void func_ov170_021edb34(Box2SysWork *syswk, u32 tray);
void func_ov170_021edecc(Box2SysWork *syswk, s32 mv);
void func_ov170_021edf5c(Box2SysWork *syswk);
void func_ov170_021ee008(Box2SysWork *syswk, u32 index, BOOL show);
void func_ov170_021ee224(Box2AppWork *app, u32 index, BOOL visible);
BOOL func_ov170_021ee234(Box2AppWork *app, u32 index);
void func_ov170_021ee244(Box2AppWork *app, u32 index, u32 dir);
void func_ov170_021ee26c(Box2AppWork *app, s32 mv);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov170_021ec8e4(Box2SysWork *syswk);
void func_ov170_021ed8a4(Box2SysWork *syswk);
// Sets an actor's animation
void func_ov170_021ebd94(Box2SysWork *syswk);
void func_ov170_021ec4e0(Box2SysWork *syswk);
void func_ov170_021ec4fc(Box2SysWork *syswk);
void func_ov170_021eca24(Box2PokeFreeWork *wk);
void func_ov170_021ecb20(Box2SysWork *syswk, u32 flags, BOOL blend);
void func_ov170_021ecb60(Box2AppWork *app, u32 iconPos, BOOL blend);
void func_ov170_021ed1c8(Box2AppWork *app);
void func_ov170_021ed2f4(Box2AppWork *app);
void func_ov170_021ed310(Box2AppWork *app);
void func_ov170_021ed508(Box2SysWork *syswk, u32 set, BOOL visible);
void func_ov170_021ed544(Box2SysWork *syswk, u32 anim);
void func_ov170_021ed55c(Box2SysWork *syswk, BOOL show);
void func_ov170_021ed594(Box2SysWork *syswk, u32 pos);
void func_ov170_021ed6e4(Box2SysWork *syswk, BOOL visible);
void func_ov170_021ed75c(Box2SysWork *syswk, u32 pos);
void func_ov170_021ed7c4(Box2AppWork *app, u32 pos);
void func_ov170_021ed918(Box2SysWork *syswk, s32 mv);
void func_ov170_021eda70(Box2AppWork *app, u32 row, s16 *x, s16 *y);
void func_ov170_021edab0(Box2SysWork *syswk, u32 tray);
void func_ov170_021edd38(Box2SysWork *syswk, s16 mv);
void func_ov170_021ee038(Box2SysWork *syswk, u8 button1, u8 button2, u8 button3);
void func_ov170_021ee0fc(Box2SysWork *syswk);
// Sets an actor's animation
void func_ov170_021ebfa8(Box2AppWork *app, u32 id, u32 anim);
void func_ov170_021ec35c(Box2SysWork *syswk, u32 tray);
void func_ov170_021ec000(Box2AppWork *app, u32 id, BOOL visible);
// Where an actor is
void func_ov170_021ec0b0(Box2AppWork *app, u32 id, s16 *x, s16 *y, u16 surface);
void func_ov170_021ed14c(Box2AppWork *app, s16 x, s16 y);
void func_ov170_021ecccc(Box2SysWork *syswk, Box2PokeInfo *info, u32 id);
void func_ov170_021ed028(Box2AppWork *app, Box2PokeInfo *info);
void func_ov170_021ecfdc(Box2AppWork *app, u8 type1, u8 type2);
u32 BoxSaveAccessor_GetPkmParam(BoxSaveAccessor *boxes, u32 tray, u32 pos, u32 param, void *buf);
void func_ov170_021ed108(Box2AppWork *app, u32 mode);

void func_ov170_021ecbd8(Box2AppWork *app, u16 pos);
void func_ov170_021ec768(Box2AppWork *app, u32 pos, u32 flag);

void func_ov170_021ec688(Box2SysWork *syswk);
void func_ov170_021ec6f4(Box2SysWork *syswk);
void func_ov170_021ecb84(Box2AppWork *app, u32 index);
void func_ov170_021ec61c(Box2SysWork *syswk);
void func_ov170_021ec5a0(Box2SysWork *syswk, BOOL flag);
void func_ov170_021ed094(Box2AppWork *app, u16 item);
void func_ov170_021ecc1c(Box2SysWork *syswk);
#endif // BWPORT_APP_BOX2_OBJ_H
