#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_sprite_app.h"

void ov112_021F13BC(Ov112SpriteApp *, u32, u32, u32);

void ov112_021F13CC(Ov112SpriteApp *, u32, u32);

void ov112_021F1488(Ov112SpriteApp *, const u16 *, u8, u32);

void ov112_021F1504(Ov112SpriteApp *, u32);

void ov112_021F1584(Ov112SpriteApp *, void *, u32, s32);

void ov112_021F15CC(Ov112SpriteApp *, u32, s32, u32, u32);

void ov112_021F1624(Ov112SpriteApp *param_1, u32 param_2, u32 param_3);

void ov112_021F135C(Ov112SpriteApp *, u32, u32, u8, u32);
void ov112_021F178C(Ov112SpriteApp *param_1);

void ov112_021F1624(Ov112SpriteApp *param_1, u32 param_2, u32 param_3) {
    u8 bVar1;
    String *string;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    Ov112Record *iVar6;
    s32 iVar5;
    u32 msgno;

    iVar6 = param_1->records[param_1->curIdx];
    uVar3 = param_1->date.month;
    uVar4 = param_1->date.day;
    iVar5 = param_1->time.hour;
    if (iVar5 < 0xc) {
        msgno = 0x57;
        if (iVar5 == 0) {
            iVar5 = 0xc;
        }
    } else {
        msgno = 0x58;
        iVar5 = iVar5 + -0xc;
        if (iVar5 == 0) {
            iVar5 = 0xc;
        }
    }
    ov112_021F1488(param_1, param_1->playerName, 0, 9);
    ov112_021F1584(param_1, iVar6->nicknameA, 1, iVar6->speciesA);
    ov112_021F1488(param_1, iVar6->playerName, 2, 9);
    ov112_021F1584(param_1, iVar6->nicknameB, 3, iVar6->speciesB);
    ov112_021F1504(param_1, iVar6->unk_E);
    ov112_021F1584(param_1, iVar6->nicknameB, 5, iVar6->speciesB);
    ov112_021F15CC(param_1, 6, iVar5, 2, 0);
    ov112_021F15CC(param_1, 7, iVar6->unk_78, 5, 0);
    ov112_021F1488(param_1, iVar6->message, 8, 0x15);
    ov112_021F15CC(param_1, 9, uVar3, 2, 0);
    ov112_021F15CC(param_1, 10, uVar4, 2, 0);
    ov112_021F13CC(param_1, param_2, param_3);
    ov112_021F15CC(param_1, 9, uVar3, 2, 1);
    ov112_021F15CC(param_1, 10, uVar4, 2, 1);
    ov112_021F13BC(param_1, 0, 0x56, 0);
    ov112_021F15CC(param_1, 6, iVar5, 2, 1);
    string = ReadMsgData_ExpandPlaceholders(param_1->msgFormat, param_1->msgData[0], msgno, param_1->heapID);
    bVar1 = GetWindowWidth(&param_1->windows[1]);
    uVar2 = FontID_String_GetCenterAlignmentX(FONT_MESSAGE, string, 0, (u32)bVar1 << 3);
    String_Delete(string);
    ov112_021F13BC(param_1, 1, msgno, uVar2 & 0xff);
    return;
}

void ov112_021F178C(Ov112SpriteApp *param_1) {
    ov112_021F135C(param_1, 3, 0x59, 4, 4);
    return;
}
