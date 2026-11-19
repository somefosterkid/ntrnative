#ifndef POKEHEARTGOLD_OVERLAY_28_H
#define POKEHEARTGOLD_OVERLAY_28_H

#include "global.h"
#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "overlay_28_dowsing_mchn.h"
#include "sprite.h"
#include "sys_task.h"
#include "touchscreen.h"

SysTask *ov28_0225D520(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void ov28_0225D5EC(BgConfig *bgConfig, SysTask *appTask);
BOOL ov28_0225D624(BgConfig *bgConfig);
void ov28_0225D628(void);

void ov28_0225D650(BgConfig *bgConfig);

void ov28_0225D6E0(BgConfig *bgConfig);
void ov28_0225D6FC(DowsingMchnApp *app, NARC *narc);
void ov28_0225D764(DowsingMchnApp *app);
void ov28_0225D7C4(DowsingMchnApp *app);

void ov28_0225D7E0(DowsingMchnApp *app);

void ov28_0225D878(DowsingMchnApp *app);
void ov28_0225D898(DowsingMchnApp *app, NARC *narc);

void ov28_0225D8D0(DowsingMchnApp *app);
void ov28_0225D8EC(DowsingMchnApp *app);
void ov28_0225D92C(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, s32 fileId, s32 arg4, s32 arg5, s32 arg6, s32 pltt_num, s32 id, s32 arg9, s32 argA, s32 argB);

void ov28_0225D9BC(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority);

void ov28_0225DA1C(DowsingMchnApp *app, s32 idx, SpriteResourcesHeader *header, const DowsingMchnSpriteDef *def);
void ov28_0225DA74(DowsingMchnApp *app);
void ov28_0225DB54(DowsingMchnApp *app);
void ov28_0225DB8C(DowsingMchnApp *app, NARC *narc);
void ov28_0225DBFC(DowsingMchnApp *app);

void ov28_0225DC2C(SysTask *task, void *env);
void DowsingMchn_GetHiddenItemLocs(DowsingMchnApp *app);
void DowsingMchn_FreeHiddenItemLocs(DowsingMchnApp *app);

void ov28_0225DD58(DowsingMchnApp *app);
void ov28_0225DE04(VecFx32 *vec, u32 deg);

u16 ov28_0225DE64(s32 x, s32 y);
u16 ov28_0225DE78(s32 x, s32 y);
u32 ov28_0225DE88(u32 angleIdx);

void ov28_0225DE98(DowsingMchnNeedle *needle);
void ov28_0225DEB0(DowsingMchnApp *app);
void ov28_0225DF04(DowsingMchnNeedle *needle);
void ov28_0225DF14(DowsingMchnApp *app);
void ov28_0225DFA4(DowsingMchnNeedle *needle);
void ov28_0225DFBC(DowsingMchnApp *app);
void ov28_0225E054(DowsingMchnNeedle *needle);
void ov28_0225E060(DowsingMchnApp *app);
void ov28_0225E0DC(DowsingMchnNeedle *needle);
void ov28_0225E0F4(DowsingMchnApp *app);

void ov28_0225E1A4(DowsingMchnApp *app);
void ov28_0225E31C(DowsingMchnApp *app, s32 delta);
void ov28_0225E374(DowsingMchnApp *app, u16 *animSeq, u16 *palIdx);
void ov28_0225E3BC(DowsingMchnApp *app);
void ov28_0225E424(Sprite *sprite, u32 deg);
void ov28_0225E43C(DowsingMchnApp *app);
s32 ov28_0225E51C(DowsingMchnApp *app, s32 x, s32 y);
void ov28_0225E578(DowsingMchnApp *app, BOOL active);
void ov28_0225E730(DowsingMchnApp *app);

s32 ov28_0225E7D4(DowsingMchnApp *app);
void ov28_0225E810(DowsingMchnApp *app);
s32 ov28_0225E8B8(const TouchscreenHitbox *hitbox, Sprite *sprite, s32 idleSeq, s32 pressSeq);
void ov28_0225E900(DowsingMchnApp *app);
void ov28_0225E938(DowsingMchnApp *app);

void ov28_0225E9E0(DowsingMchnApp *app);
s32 ov28_0225EA58(DowsingMchnApp *app);

#endif // POKEHEARTGOLD_OVERLAY_28_H
