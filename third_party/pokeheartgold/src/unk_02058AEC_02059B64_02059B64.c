#include "global.h"

#include "constants/heap.h"

#include "field/fieldmap.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "launch_application.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_trainer_card.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "system.h"
#include "task.h"
#include "unk_02034354.h"
#include "unk_02058034.h"
#include "union_room_card_task.h"

void sub_02056E60(void);

void sub_02057F70(void);

BOOL sub_02059B64(TaskManager *param_1);

BOOL sub_02059B64(TaskManager *param_1) {
    u8 bVar1;
    u8 uVar2;
    FieldSystem *fieldSystem;
    UnionRoomPeerCardTask *env;
    u8 *src;
    MessageFormat *pMVar3;
    MsgData *pMVar4;
    String *pSVar5;
    PlayerProfile *playerProfile;
    Options *pOVar6;
    BOOL BVar7;
    s32 strno;
    fieldSystem = TaskManager_GetFieldSystem(param_1);
    env = TaskManager_GetEnvironment(param_1);
    src = sub_020588DC(env->netId, 0, 0);
    switch (env->state) {
    case 0:
        pMVar3 = MessageFormat_New(HEAP_ID_FIELD1);
        env->msgFormat = pMVar3;
        pMVar4 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xe, HEAP_ID_FIELD1);
        env->msgData = pMVar4;
        pSVar5 = String_New(200, HEAP_ID_FIELD1);
        env->message = pSVar5;
        pSVar5 = String_New(200, HEAP_ID_FIELD1);
        env->formattedMessage = pSVar5;
        switch (*src) {
        case 7:

        case 8:
            strno = ((u8)src[3]) + 2;
            break;

        case 10:
            strno = 0x19;
            break;

        case 0xb:
            strno = 0x1a;
            break;

        case 0xc:
            strno = 0x1b;
            break;

        default:
            strno = 2;
            break;
        }

        ReadMsgDataIntoString(env->msgData, strno, env->message);
        playerProfile = sub_02034818(env->netId);
        BufferPlayersName(env->msgFormat, 0, playerProfile);
        StringExpandPlaceholders(env->msgFormat, env->formattedMessage, env->message);
        DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &env->window, GF_BG_LYR_MAIN_3);
        pOVar6 = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        DialogBox_LoadFrame(&env->window, pOVar6);
        pOVar6 = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        bVar1 = DialogBox_PrintMessage(&env->window, env->formattedMessage, pOVar6, 1);
        env->printerId = bVar1;
        env->state = env->state + 1;
        break;

    case 1:
        uVar2 = DialogBox_IsPrintFinished((u8)env->printerId);
        if ((uVar2 != 0) && ((((u32 *)(&gSystem))[0x12] & 1) != 0)) {
            DestroyMsgData(env->msgData);
            MessageFormat_Delete(env->msgFormat);
            String_Delete(env->message);
            String_Delete(env->formattedMessage);
            ClearFrameAndWindow2(&env->window, 0);
            RemoveWindow(&env->window);
            FieldMap_FadeScreen(0);
            env->state = env->state + 1;
        }
        break;

    case 2:
        BVar7 = IsPaletteFadeFinished();
        if (BVar7 != 0) {
            env->state = env->state + 1;
        }
        break;

    case 3:
        MI_CpuCopy8(src, &env->args, sizeof(TrainerCase));
        TrainerCard_LaunchApp(fieldSystem, &env->args);
        env->state = env->state + 1;
        break;

    case 4:
        BVar7 = FieldSystem_ApplicationIsRunning(fieldSystem);
        if (BVar7 == 0) {
            env->state = env->state + 1;
        }
        break;

    case 5:
        FieldSystem_LoadFieldOverlay(fieldSystem);
        env->state = env->state + 1;
        break;

    case 6:
        BVar7 = sub_020505C8(fieldSystem);
        if (BVar7 == 0) {
            FieldMap_FadeScreen(1);
            sub_02056E60();
            env->state = env->state + 1;
        }
        break;

    case 7:
        sub_02057F70();
        Heap_Free(env);
        return 1;

    default:
        return 1;
    }

    return 0;
}
