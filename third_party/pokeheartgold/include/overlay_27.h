#ifndef POKEHEARTGOLD_OVERLAY_27_H
#define POKEHEARTGOLD_OVERLAY_27_H

#include "global.h"
#include "sys_task.h"
#include "field_types_def.h"
#include "bg_window.h"
#include "pm_string.h"
#include "overlay_27_ov27task_data.h"

struct Ov27Task;
struct Ov27PhoneIcon;

SysTask *ov27_02259F80(BgConfig *bgConfig, u16 *arg1, FieldSystem *fieldSystem);

void ov27_0225A2CC(SysTask *arg0);

void ov27_0225A2EC(SysTask *arg0, s32 arg1);

void ov27_0225A320(SysTask *sysTask, void *data);

void ov27_0225A4B8(struct Ov27Task *arg0);
s32 ov27_0225A4D0(struct Ov27Task *arg0);

s32 ov27_0225A714(struct Ov27Task *arg0);
u32 ov27_0225A7B0(struct Ov27Task *arg0);
void ov27_0225A7DC(struct Ov27Task *arg0);

s32 ov27_0225A89C(FieldSystem *arg0);

u8 ov27_0225AA60(u8 *arg0, s32 arg1);

void ov27_0225AA7C(struct Ov27Task *arg0);

void ov27_0225AD0C(struct Ov27Task *arg0);

s32 ov27_0225AE8C(s32 arg0);

void ov27_0225B404(struct Ov27Task *arg0);

void ov27_0225BB38(Window *arg0, String *arg1, s32 arg2);

void ov27_0225BC34(struct Ov27Task *arg0);

s32 ov27_0225BD50(FieldSystem *arg0);

void ov27_0225BDDC(struct Ov27PhoneIcon *arg0, struct Ov27Task *arg1);

void ov27_0225BEB0(struct Ov27PhoneIcon *arg0);

void ov27_0225C044(struct Ov27Task *arg0);
void ov27_0225C06C(struct Ov27Task *arg0);
void ov27_0225C088(struct Ov27Task *arg0);

void ov27_0225C1EC(struct Ov27Task *param_1);

void ov27_0225C238(void);

void ov27_0225C248(void);
void ov27_0225C24C(void);

void ov27_0225C398(BgConfig *arg0, SysTask *arg1);

struct Ov27Task;
BOOL ov27_0225BDC8(struct Ov27Task *arg0);

// Prints msgId (expanded through task's MessageFormat) into a temporary window at (x, y) of size width x height tiles;
// textX == -1 centres the text in the window. asm/overlay_27_0225A2CC_0225BED8.s (DS); C body under SDK_PORT in overlay_27_0225BF84.c.
void ov27_0225BED8(struct Ov27Task *task, s32 msgId, s32 x, s32 y, s32 width, s32 height, s32 baseTile, s32 textX, s32 textY);

void ov27_0225C41C(SysTask *task, Ov27TouchMenuCallback callback, struct FieldMenuManager *menu);

void ov27_0225C4AC(Ov27TaskData *data);
void ov27_0225C540(Ov27TaskData *data);

void ov27_0225C72C(Ov27TaskData *data);

void ov27_0225C914(Ov27TaskData *data, u32 unused);
void ov27_0225C930(Ov27TaskData *data);
void ov27_0225C8D0(Window *window, struct MsgData *msgData, s32 strno);
s32 ov27_0225C944(Ov27TaskData *data);

s32 ov27_0225C94C(Ov27TaskData *data);

s32 ov27_0225C988(Ov27TaskData *data);
s32 ov27_0225C994(Ov27TaskData *data);
s32 ov27_0225C9E4(Ov27TaskData *data);

s32 ov27_0225C9F8(Ov27TaskData *data);

s32 ov27_0225CA14(Ov27TaskData *data);
s32 ov27_0225CA68(s32 *selection, s32 nItems, s32 dir);

s32 ov27_0225CC90(Ov27TaskData *data);

s32 ov27_0225CCBC(Ov27TaskData *data);

void ov27_0225CD74(Ov27TaskData *data, u32 nItems);

void ov27_0225CEAC(Ov27TaskData *data);
void ov27_0225C5E4(Ov27TaskData *data);
void ov27_0225C6F8(Ov27TaskData *data);
void ov27_0225C80C(Ov27TaskData *data, s32 pos);
s32 ov27_0225C9CC(Ov27TaskData *data);
s32 ov27_0225CA98(Ov27TaskData *data);
void ov27_0225CCE0(Ov27TaskData *data, s32 pos);
void ov27_0225CD18(Ov27TaskData *data, s32 nItems, s32 selection);
s32 ov27_0225CD94(Ov27TaskData *data);
void ov27_0225C434(SysTask *task, void *taskData);
SysTask *ov27_0225C250(BgConfig *bgConfig, u16 *touchInput, FieldSystem *fieldSystem, u16 *result);

#endif // POKEHEARTGOLD_OVERLAY_27_H
