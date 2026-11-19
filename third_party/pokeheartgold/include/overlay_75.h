#ifndef POKEHEARTGOLD_OVERLAY_75_H
#define POKEHEARTGOLD_OVERLAY_75_H

#include "global.h"
#include "overlay_manager.h"
#include "sys_task.h"
#include "list_menu.h"

struct UnkStruct_ov98_02246E88;
struct UnkStruct_ov98_02247704;

s32 ov75_02246960(OverlayManager *arg0);

s32 ov75_02246B00(OverlayManager *arg0);

u8 ov75_02246BE8(struct UnkStruct_ov98_02246E88 *arg0);

void ov75_02246D40(struct UnkStruct_ov98_02246E88 *arg0);

void *ov75_02246EAC(struct UnkStruct_ov98_02246E88 *arg0);

void ov75_022476E8(SysTask *param0, void *param1);

s32 ov75_02247A78(struct UnkStruct_ov98_02247704 *arg0);
s32 ov75_02247A98(struct UnkStruct_ov98_02247704 *arg0);
s32 ov75_02247B98(struct UnkStruct_ov98_02247704 *arg0);

s32 ov75_0224852C(void *arg0);

s32 ov75_02248684(void *arg0);

typedef struct UnkStruct_ov75_02248B8C UnkStruct_ov75_02248B8C;
s32 ov75_02248B8C(UnkStruct_ov75_02248B8C *arg0);
s32 ov75_02248C64(void *arg0);
s32 ov75_02248C84(void *arg0);

typedef struct UnkStruct_ov75_02249258 UnkStruct_ov75_02249258;
s32 ov75_02249258(UnkStruct_ov75_02249258 *arg0);

void ov75_02249758(ListMenu *param0, u32 param1, u8 param2);
void ov75_0224976C(ListMenu *param0, u32 param1, u8 param2);

void ov75_02249884(const char *param0, u32 *param1, int param2);

#endif // POKEHEARTGOLD_OVERLAY_75_H
