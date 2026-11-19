#ifndef POKEHEARTGOLD_OVERLAY_34_H
#define POKEHEARTGOLD_OVERLAY_34_H

#include "global.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_34_work.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_palpad.h"
#include "sprite.h"
#include "sys_task.h"

void ov34_0225D520(Ov34Work *work);
void ov34_0225D558(Ov34Work *work, BgConfig *bgConfig);
void ov34_0225D5A0(SysTask *task, void *data);
void ov34_0225D5F8(Ov34Work *work);
void ov34_0225D650(BgConfig *bgConfig, Ov34Row *rows, Window *titleWindow);
void ov34_0225D77C(Ov34Row *rows, Window *titleWindow);
Ov34Work *ov34_0225D7A8(FieldSystem *fieldSystem);
void ov34_0225D87C(Ov34Work *work);
void ov34_0225D900(BgConfig *bgConfig);
void ov34_0225D924(BgConfig *bgConfig);
void ov34_0225DA50(Ov34Work *work);
void ov34_0225DB20(Ov34Work *work);
int ov34_0225DC00(Ov34ChatLog *chatLog, int index);
s32 ov34_0225DC0C(s32 arg0, s32 arg1);
void ov34_0225DC18(Ov34Work *work, int row, Ov34ChatEntry *entry);
void ov34_0225DD04(Ov34Work *work);
void ov34_0225DDB8(Sprite *sprite, int y);
void ov34_0225DE04(Ov34Work *work);
s32 ov34_0225DE94(Ov34Work *work);
int ov34_0225E020(Ov34Work *work);
void ov34_0225E0E4(Ov34Work *work);
void ov34_0225E164(Ov34Work *work);
String *ov34_0225E2BC(SavePalPad *palPad, u32 id, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile);
void ov34_0225E1C4(BgConfig *bgConfig, NNSG2dScreenData *scrnData, Ov34Row *rows, int selected, int count, int *total);
void ov34_0225E348(Ov34Work *work, u32 id, MailMessage *mail, PlayerProfile *profile);
s32 ov34_0225E428(Ov34Work *work, MailMessage *mail, s32 id);
void ov34_0225E4A8(Ov34Work *work, PlayerProfile *profile, MailMessage *mail, s32 id);
void ov34_0225E4F8(Ov34Work *work);
void ov34_0225E560(Ov34Work *work);
void ov34_0225E56C(Ov34Work *work);
void ov34_0225E58C(Ov34Work *work);
u8 ov34_0225E5D4(Ov34Work *work);
void ov34_0225E5DC(Ov34Work *work, s32 value);
s32 ov34_0225E5E4(Ov34Work *work);
void ov34_0225E5EC(Ov34Work *work, int index);
void ov34_0225E630(Ov34Work *work);

#endif // POKEHEARTGOLD_OVERLAY_34_H
