#include "global.h"
struct BattleCastleSelfApp;

#include "bg_window.h"
#include "overlay_80_02237A70.h"
#include "player_avatar.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "unk_0205BFF0.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_0223FCB4(void *, Window *, s32, u32, s32, s32, u8, u8, u8, u8);

s32 ov83_02240C48(void *, s32, s32, s32, s32);

s32 ov83_02240F7C(void *, void *, s32 *, void *, s32 *);

s32 ov83_02241DD8(void *, Window *, MsgData *, s32, s32, s32, s32, s32, s32);

void ov83_02241770(struct BattleCastleSelfApp *arg0, Window *arg1);

void ov83_02241770(BattleCastleSelfApp *arg0, Window *arg1) {
    u8 sp20[4];
    u8 sp1C[4];
    u32 sp18;
    u16 var_r7;
    u16 temp_r7;
    u32 temp_r7_2;
    u16 var_r6;
    u16 temp_r6;
    u32 temp_r6_2;

    ov83_02240F7C(arg0, &sp20[2], (s32 *)sp20, &sp1C[2], (s32 *)sp1C);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        temp_r7 = *(u16 *)((u8 *)((s32 *)sp1C) + 6) + 0x48;
        temp_r6 = *(u16 *)((u8 *)((s32 *)sp1C) + 4) + 1;
        FillWindowPixelRect(arg1, 0U, temp_r7, temp_r6, 0x30U, 0x10U);
        sp18 = sub_0205C1F0(arg0->challengeType);
        ov83_02240C48(arg0, 0, FrontierSave_GetStat(arg0->frontier, (s32)sp18, (s32)sub_0205C268(sub_0205C1F0(arg0->challengeType))), 4, 0);
        *((s8 *)&arg0->printerID) = ov83_0223FCB4(arg0, arg1, 2, temp_r7, (s32)temp_r6, 0xFF, 1, 2, 0, 0);
    } else {
        FillWindowPixelRect(arg1, 0U, 0x40U, 0U, 0x30U, 0x10U);
        FillWindowPixelRect(arg1, 0U, 0xC0U, 0U, 0x30U, 0x10U);
        if (sub_0203769C() == 0) {
            temp_r6_2 = sub_0205C1F0(arg0->challengeType);
            var_r6 = FrontierSave_GetStat(arg0->frontier, (s32)temp_r6_2, (s32)sub_0205C268(sub_0205C1F0(arg0->challengeType)));
            var_r7 = arg0->partnersCP;
        } else {
            var_r6 = arg0->partnersCP;
            temp_r7_2 = sub_0205C1F0(arg0->challengeType);
            var_r7 = FrontierSave_GetStat(arg0->frontier, (s32)temp_r7_2, (s32)sub_0205C268(sub_0205C1F0(arg0->challengeType)));
        }
        ov83_02240C48(arg0, 0, var_r6, 4, 0);
        ov83_02241DD8(arg0, arg1, *((void **)&arg0->msgLoader), 2, 0x70, 0, 0, 0x10200, 1);
        ov83_02240C48(arg0, 0, var_r7, 4, 0);
        ov83_02241DD8(arg0, arg1, *((void **)&arg0->msgLoader), 3, 0xF0, 0, 0, 0x10200, 1);
    }
    ScheduleWindowCopyToVram(arg1);
}
