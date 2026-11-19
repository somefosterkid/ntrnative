#ifndef POKEHEARTGOLD_OVERLAY_82_H
#define POKEHEARTGOLD_OVERLAY_82_H

#include "global.h"
#include "overlay_manager.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "sys_task.h"

s32 ov82_0223DF74(OverlayManager *arg0);

void ov82_0223E070(u8 *arg0);

void ov82_0223E8C4(void *arg0);

void ov82_0223EB3C(void *arg0);
void ov82_0223EB9C(void *arg0);

void ov82_0223EECC(s32 param_1, GFBgLayer param_2);

void ov82_0223F10C(void *arg0, Window *arg1, u32 arg2, u32 arg3, u32 arg4);

void ov82_0223F224(void *arg0);

s32 ov82_0223F53C(void *arg0);

void ov82_0223F74C(void *arg0, s16 arg1);
void ov82_0223F764(int netID, int unused, void *unused2, void *unused3);
void ov82_0223F770(void *arg0, s16 arg1, s16 arg2);

void ov82_0223F7B4(s32 netID, s32 unused, void *data, void *context);

void ov82_0223F808(void *arg0, s16 arg1, s16 arg2);

void ov82_0223F814(s32 netID, s32 unused, void *data, void *context);

void ov82_0223F84C(void *arg0);

void ov82_0223F8E4(void *arg0);

void ov82_0223F948(s32 arg0);

SysTask *ov82_0223FDB8(SysTaskFunc callback, void *param, int priority);

SysTask *ov82_0223FE18(SysTaskFunc callback, void *param, int priority);

#endif // POKEHEARTGOLD_OVERLAY_82_H
