#include "global.h"

#include "unk_0203170C.h"
#include "unk_02037C94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov72_02237CB0(void);

s32 ov73_021E8EBC(WifiBattleTowerAppState *arg0);

s32 ov73_021E8EBC(WifiBattleTowerAppState *arg0) {
    ov72_02237CB0();
    arg0->state = NINTENDO_WFC_STATE_WAIT_INFO;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021EA180(void *);

s32 ov73_021E8ED8(WifiBattleTowerAppState *arg0);

s32 ov73_021E8ED8(WifiBattleTowerAppState *arg0) {
    s32 v0;
    s32 v1;
    u32 v2;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v2 = v0 + 7;
        switch (v2) {
        case 7:
            ov73_021EA180(arg0);
            v1 = arg0->args->mode;
            switch (v1) {
            case 0:
                ov73_021E83EC(arg0, 0xB, 0xD);
                break;
            case 1:
                arg0->state = NINTENDO_WFC_STATE_PRINT_SENDING_RECORD;
                break;
            case 2:
                arg0->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
                break;
            }
            break;
        case 8:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 6:
        case 9:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 0:
        case 3:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 1:
        case 5:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        default:
            ov73_021EA180(arg0);
            sub_020399EC();
            break;
        }
    } else {
        arg0->timeoutCounter = (s32)(arg0->timeoutCounter + 1);
        if (arg0->timeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov72_02237CF4(void *, void *);

s32 ov73_021E8FB0(WifiBattleTowerAppState *arg0);

s32 ov73_021E8FB0(WifiBattleTowerAppState *arg0) {
    sub_0203189C(arg0->args->saveData, (void *)&arg0->weTrainer);
    ov72_02237CF4(&arg0->weTrainer, &arg0->weTrainerErr);
    arg0->state = NINTENDO_WFC_STATE_WAIT_SET_PROFILE;
    arg0->timeoutCounter = 0;
    return 3;
}
