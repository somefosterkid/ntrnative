#include "global.h"

#include "list_menu_items.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02242AEC(BattleCastleSelfApp *, u8, u32, u32, u8);

u8 ov83_0224777C(SaveData *, u8, u8);

extern const BattleCastleMenuEntry ov83_02247F4C[];

extern const u8 ov83_02248008[];

void ov83_02242BF0(BattleCastleSelfApp *arg0);

void ov83_02242BF0(BattleCastleSelfApp *arg0) {
    u32 temp_r3;
    u32 temp_r7;
    u8 var_r5;
    u8 var_r6;
    const BattleCastleMenuEntry *temp_r2;

    arg0->listMenuItems = ListMenuItems_New(5U, HEAP_ID_107);
    temp_r7 = ov83_0224777C(arg0->saveData, arg0->challengeType, 0);
    var_r5 = 0;
    var_r6 = 0;
    do {
        temp_r2 = &ov83_02247F4C[var_r6];
        temp_r3 = temp_r2->action;
        switch (temp_r3) {
        case 1:
        case 2:
        case 3:
            if (temp_r7 >= temp_r2->minRank) {
                ListMenuItems_AppendFromMsgData(arg0->listMenuItems, arg0->msgLoader, temp_r2->msgID, (s32)temp_r3);
                var_r5 += 1;
            }
            break;
        case 4:
            if (temp_r7 != 3) {
                ListMenuItems_AppendFromMsgData(arg0->listMenuItems, arg0->msgLoader, temp_r2->msgID, (s32)temp_r3);
                var_r5 += 1;
            }
            break;
        default:
            ListMenuItems_AppendFromMsgData(arg0->listMenuItems, arg0->msgLoader, temp_r2->msgID, (s32)temp_r3);
            var_r5 += 1;
            break;
        }
        var_r6 += 1;
    } while ((u32)var_r6 < 5U);
    ov83_02242AEC(arg0, var_r5, 0x11, ov83_02248008[var_r5], 0xD);
}

extern const BattleCastleMenuEntry ov83_02247EB0[];

void ov83_02242CAC(BattleCastleSelfApp *arg0);

void ov83_02242CAC(BattleCastleSelfApp *arg0) {
    s32 temp_r3;
    u32 temp_r7;
    u8 var_r4;
    ListMenuItem **new_var;
    u8 var_r6;
    const BattleCastleMenuEntry *temp_r2;
    arg0->listMenuItems = ListMenuItems_New(4U, HEAP_ID_107);
    temp_r7 = ov83_0224777C(arg0->saveData, arg0->challengeType, 1);
    var_r4 = 0;
    var_r6 = 0;
    do {
        temp_r2 = &ov83_02247EB0[var_r6];
        temp_r3 = temp_r2->action;
        new_var = &(arg0->listMenuItems);
        switch (temp_r3) {
        case 6:

        case 7:
            if (temp_r7 >= temp_r2->minRank) {
                ListMenuItems_AppendFromMsgData(*new_var, arg0->msgLoader, temp_r2->msgID, temp_r3);
                var_r4 += 1;
            }
            break;

        case 8:
            if (temp_r7 != 3) {
                ListMenuItems_AppendFromMsgData(*new_var, arg0->msgLoader, temp_r2->msgID, temp_r3);
                var_r4 += 1;
            }
            break;

        default:
            ListMenuItems_AppendFromMsgData(*new_var, arg0->msgLoader, temp_r2->msgID, temp_r3);
            var_r4 += 1;
            break;
        }

        var_r6 += 1;
    } while (((u32)var_r6) < 4U);
    ov83_02242AEC(arg0, var_r4, 0x11, ov83_02248008[var_r4], 0xD);
}
