#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_74_mystery_gift_statics.h"
#include "overlay_74_mystery_gift_templates.h"
#include "overlay_manager.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_mystery_gift_animation.h"

void ov74_022352A0(enum HeapID heapID);

s32 ov74_0222CE10(void *);

s32 ov74_022358C8(s32);

void ov74_0222AB70(OverlayManager *arg0, void *arg1);

void ov74_02229F04(void *arg0);

void ov74_0223539C(s32 arg0, s32 arg1, void *arg2, s32 arg3);

void ov74_022353FC(void *arg0);

void ov74_0222AB0C(OverlayManager *arg0, int *arg1, void *arg2);

void ov74_0222CEE0(void *arg0);

void ov74_0222CEC0(void);

void *ov74_0222A078(OverlayManager *arg0);

void ov74_02229E28(void *arg0, s32 arg1);

s32 ov74_0222FD98(void *arg0, void *arg1);

void *ov74_02236988(void);

void ov74_0222AAAC(OverlayManager *arg0, void *arg1, void *arg2);

void ov74_0222AA18(OverlayManager *arg0, void *arg1, s32 arg2);

void ov74_0222A43C(OverlayManager *arg0);

void ov74_02229F28(void *arg0, s32 arg1);

s32 ov74_0222A0E4(void *arg0);

void ov74_0222A174(OverlayManager *arg0);

void ov74_02229F60(void *arg0, u8 arg1);

void ov74_02235DC4(void *arg0, void *arg1);

s32 ov74_0223615C(void);

void ov74_02236128(void);

void ov74_02229F78(void *arg0, u8 arg1);

void ov74_0222A240(OverlayManager *arg0, s32 arg1);

u8 ov74_02229DBC(void);

void ov74_02229DF8(void);

void ov74_02236140(void);

s32 ov74_0222ADBC(OverlayManager *arg0, void *arg1, s32 arg2, s32 arg3);

void ov74_02235258(void *arg0, s32 arg1);

s32 ov74_0222AD6C(OverlayManager *arg0);

void ov74_0222AE3C(void *arg0);

void ov74_0222A94C(OverlayManager *arg0, s32 arg1, s32 arg2);

s32 ov74_0222A5AC(OverlayManager *arg0);

s32 ov74_0222A494(OverlayManager *arg0);

s32 ov74_02229D6C(void *arg0);

void ov74_02229E68(OverlayManager *arg0);

void ov74_02235390(s32 arg0);

void ov74_0222A7A0(void *arg0);

void ov74_0222FC50(void *arg0, void *arg1, s32 arg2);

void ov74_0222EC08(void *arg0, void *arg1, s32 arg2);

void ov74_0222C04C(SysTask *arg0, void *arg1);

s32 ov74_0222A6C0(OverlayManager *man);

void ov74_0222A2A4(OverlayManager *man);

void ov74_0222A538(void);

BOOL ov74_0222C2EC(OverlayManager *arg0, int *arg1) {
    s32 (*temp_fn)(void *);
    s32 temp_r1;
    s32 temp_r0;
    s32 temp_r7;
    void *temp_r6;
    MysteryGiftAppData *temp_r4;
    u32 state;

    temp_r4 = OverlayManager_GetData(arg0);
    CTRDG_IsExisting();
    if (temp_r4->unk_5C8 != 0x1D) {
        ov74_0222CE10(temp_r4);
    }
    state = *arg1;
    switch (state) {
    case 0:
        ov74_022352A0(HEAP_ID_84);
        temp_r4->statePtr = arg1;
        *arg1 = 1;
        break;
    case 1:
        ov74_0222AB70(arg0, temp_r4);
        ov74_02229F04(temp_r4);
        ov74_0223539C(1, 3, arg1, 2);
        break;
    case 2:
        ov74_022353FC(arg1);
        break;
    case 3:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A6C0);
        break;
    case 4:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A5AC);
        break;
    case 5:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A2A4);
        break;
    case 27:
        ov74_0222AC1C(arg0, arg1);
        break;
    case 32:
        ov74_0222CEE0(temp_r4);
        if (ov74_0223D0A8.state == WIRELESS_DISTRIBUTION_STATE_45) {
            ov74_02229E28(temp_r4, 0);
            ov74_0222A078(arg0);
            temp_r4->canReceiveGiftStatus = ov74_0222FD98(temp_r4->saveData, &temp_r4->eventData);
            if (temp_r4->canReceiveGiftStatus == 1) {
                ov74_02229E28(temp_r4, 0);
                *arg1 = 0x31;
            } else {
                if (!WindowIsInUse(&temp_r4->wonderCardTitleWindow)) {
                    AddWindowParameterized(temp_r4->bgConfig, &temp_r4->wonderCardTitleWindow, 0, 3, 2, 0x1A, 4, 0, 0x1C4);
                }
                ov74_0222AAAC(arg0, &temp_r4->wonderCardTitleWindow, ov74_02236988());
                ov74_0222AA18(arg0, &temp_r4->messageBox, 4);
                ov74_0222A43C(arg0);
                *arg1 = 0x1F;
            }
        }
        temp_r1 = ov74_0223D0A8.state;
        if ((u32)(temp_r1 - 0x2E) <= 1) {
            ov74_0222CEC0();
            *arg1 = 0x11;
        }
        break;
    case 31:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A538);
        ov74_0222CEE0(temp_r4);
        break;
    case 33:
        temp_r6 = ov74_0222A078(arg0);
        ov74_02229E28(temp_r4, 1);
        ov74_02229F28(temp_r4, 1);
        if (ov74_0222A0E4(temp_r6) == 1) {
            MysteryGiftAnimationManager *animMan;

            ov74_0222A174(arg0);
            temp_r4->animationStatus = 1;
            ov74_02229F28(temp_r4, -1);
            ov74_02229F60(&temp_r4->wonderCardTitleWindow, 0);
            animMan = Heap_Alloc(HEAP_ID_84, sizeof(MysteryGiftAnimationManager));
            memset(animMan, 0, sizeof(MysteryGiftAnimationManager));
            animMan->animationStatusPtr = &temp_r4->animationStatus;
            animMan->spriteMan = ov74_02235708();
            SysTask_CreateOnMainQueue(ov74_0222C04C, animMan, 5);
        } else {
            ov74_02235DC4(temp_r4->bgConfig, temp_r6);
            ov74_0222A174(arg0);
        }
        *arg1 = 0x22;
        break;
    case 35:
        if ((s32)temp_r4->animationStatus != 0) {
            break;
        }
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 8, 0x38);
        GfGfx_EngineATogglePlanes(1, 1);
        break;
    case 34:
        temp_r7 = ov74_0223615C();
        if (temp_r7 == 4) {
            ov74_02236128();
        }
        if (temp_r7 == 2) {
            ov74_02229E28(temp_r4, 0);
            ov74_02229F28(temp_r4, -1);
            ov74_02229F78(&temp_r4->messageBox, 0);
            BgClearTilemapBufferAndCommit(temp_r4->bgConfig, 0);
            GfGfx_EngineATogglePlanes(1, 0);
            temp_r6 = &temp_r4->eventData.wonderCard;
            if (ov74_0222A0E4(temp_r6) == 1) {
                ov74_02235DC4(temp_r4->bgConfig, temp_r6);
                temp_r4->animationStatus = 2;
            } else {
                PlaySE(SEQ_SE_DP_UG_020);
            }
            *arg1 = 0x23;
            break;
        }
        if (temp_r7 != 3) {
            break;
        }
        ov74_02229E28(temp_r4, 0);
        ov74_02229F28(temp_r4, -1);
        temp_r4->eventData.header.unk8E_2 = 0;
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 0x1B, 0x38);
        break;
    case 36:
        ov74_0222CEE0(temp_r4);
        temp_r1 = ov74_0223D0A8.state;
        if ((u32)(temp_r1 - 0x2D) > 2) {
            break;
        }
        if (temp_r1 == 0x2F) {
            ov74_0222CEC0();
        }
        *arg1 = ov74_0222A494(arg0);
        break;
    case 7:
        temp_r4->unk_438 = ov74_02229D6C(temp_r4);
        if (temp_r4->unk_438 != -1) {
            ov74_02229E28(temp_r4, 0);
            temp_r4->canReceiveGiftStatus = ov74_0222FD98(temp_r4->saveData, &temp_r4->eventData);
            if (!WindowIsInUse(&temp_r4->wonderCardTitleWindow)) {
                AddWindowParameterized(temp_r4->bgConfig, &temp_r4->wonderCardTitleWindow, 0, 3, 2, 0x1A, 4, 0, 0x1C4);
            }
            ov74_0222AAAC(arg0, &temp_r4->wonderCardTitleWindow, &temp_r4->eventData);
            ov74_0222AA18(arg0, &temp_r4->messageBox, 4);
            ov74_0222A240(arg0, 0);
            *arg1 = 8;
        }
        if (!(gSystem.newKeys & 2)) {
            if (--temp_r4->wirelessCommsTimeout != 0) {
                break;
            }
        }
        ov74_02229E28(temp_r4, 0);
        ov74_02229DF8();
        sub_0203A914();
        *arg1 = 0x11;
        break;
    case 8:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A494);
        break;
    case 9:
        temp_r7 = sub_0203769C();
        if (temp_r7 != 0 && sub_020373B4((u16)temp_r7) != 0) {
            if (sub_02037B38(0xAB) == 1) {
                ov74_02229E28(temp_r4, 0);
                sub_020398D4(1, 1);
                ov74_0222AA18(arg0, &temp_r4->messageBox, 7);
                ov74_02229F28(temp_r4, 1);
                ov74_02229E28(temp_r4, 1);
                temp_r4->wirelessCommsTimeout = 0x258;
                *arg1 = 0xA;
            }
        } else if (temp_r7 != 0 && sub_020373B4((u16)temp_r7) == 0) {
            ov74_02229DF8();
            sub_0203A914();
            ov74_02229F28(temp_r4, -1);
            ov74_02229E28(temp_r4, 0);
            *arg1 = 0x10;
            break;
        }
        if (!(gSystem.newKeys & 2)) {
            if (--temp_r4->wirelessCommsTimeout != 0) {
                break;
            }
        }
        ov74_02229DF8();
        sub_0203A914();
        ov74_02229F28(temp_r4, -1);
        ov74_02229E28(temp_r4, 0);
        *arg1 = 0x11;
        break;
    case 10:
        if (ov74_02229DBC() != 0) {
            *arg1 = 0xB;
        }
        temp_r0 = sub_0203769C();
        if (!(gSystem.newKeys & 2)) {
            if (--temp_r4->wirelessCommsTimeout != 0) {
                if (temp_r0 == 0) {
                    break;
                }
                if (sub_020373B4((u16)temp_r0) != 0) {
                    break;
                }
            }
        }
        ov74_02229E28(temp_r4, 0);
        ov74_02229DF8();
        sub_0203A914();
        *arg1 = 0x11;
        break;
    case 11:
        temp_r6 = ov74_0222A078(arg0);
        ov74_02235DC4(temp_r4->bgConfig, temp_r6);
        sub_020398D4(0, 0);
        ov74_0222A174(arg0);
        *arg1 = 0xC;
        break;
    case 12:
        temp_r0 = sub_0203769C();
        if (temp_r0 != 0 && sub_020373B4((u16)temp_r0) == 0) {
            ov74_02229DF8();
            temp_r4->cancelSave = 1;
            ov74_02236140();
            ov74_02229E28(temp_r4, 0);
            sub_0203A914();
            ov74_02229F28(temp_r4, -1);
            *arg1 = 0xE;
            break;
        }
        if (ov74_0223615C() == 4) {
            sub_02037AC0(0x93);
            *arg1 = 0xD;
            temp_r4->delay = 0x78;
            break;
        }
        if (ov74_0223615C() != 3) {
            break;
        }
        ov74_02236140();
        temp_r4->eventData.header.unk8E_2 = 0;
        ov74_02229E28(temp_r4, 0);
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 0x1B, 0x38);
        break;
    case 13:
        if (sub_02037B38(0x93) == 1) {
            ov74_02236128();
            temp_r4->delay = 0xA;
            *arg1 = 0xF;
            break;
        }
        if (--temp_r4->delay == 0) {
            ov74_02236140();
            temp_r4->eventData.header.unk8E_2 = 0;
            ov74_02229E28(temp_r4, 0);
            *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 0x1B, 0x38);
        }
        break;
    case 14:
        ov74_02229F60(&temp_r4->wonderCardTitleWindow, 0);
        temp_r4->eventData.header.unk8E_2 = 0;
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 0x1D, 0x38);
        break;
    case 15:
        if (--temp_r4->delay != 0) {
            break;
        }
        ov74_02229E28(temp_r4, 0);
        ov74_02229DF8();
        sub_0203A914();
        PlaySE(SEQ_SE_DP_UG_020);
        ov74_02229F28(temp_r4, -1);
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 8, 0x38);
        break;
    case 16:
        PlaySE(SEQ_SE_DP_SELECT);
        ov74_0222AA18(arg0, &temp_r4->messageBox, 0x19);
        ov74_02229F60(&temp_r4->wonderCardTitleWindow, 0);
        *arg1 = 0x12;
        break;
    case 17:
        if (temp_r4->giftSearchResultDelay != 0) {
            temp_r4->giftSearchResultDelay -= 1;
            break;
        }
        ov74_02229E28(temp_r4, 0);
        PlaySE(SEQ_SE_DP_SELECT);
        ov74_0222AA18(arg0, &temp_r4->messageBox, 0x18);
        ov74_02229F60(&temp_r4->wonderCardTitleWindow, 0);
        *arg1 = 0x12;
        break;
    case 18:
        if (gSystem.newKeys == 0) {
            break;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        ov74_0222A94C(arg0, 0xC4, 0);
        *arg1 = 3;
        break;
    case 19:
        if (temp_r4->giftSearchResultDelay != 0) {
            temp_r4->giftSearchResultDelay -= 1;
            break;
        }
        if (temp_r4->foundGiftType != 0x1B) {
            ov74_02235258(&temp_r4->eventData, 0x3A8);
        }
        temp_r4->canReceiveGiftStatus = ov74_0222FD98(temp_r4->saveData, &temp_r4->eventData);
        if (temp_r4->canReceiveGiftStatus == 1) {
            ov74_02229E28(temp_r4, 0);
            *arg1 = 0x31;
            break;
        }
        if (!WindowIsInUse(&temp_r4->wonderCardTitleWindow)) {
            AddWindowParameterized(temp_r4->bgConfig, &temp_r4->wonderCardTitleWindow, 0, 3, 2, 0x1A, 4, 0, 0x1C4);
        }
        ov74_0222AAAC(arg0, &temp_r4->wonderCardTitleWindow, &temp_r4->eventData);
        ov74_0222AA18(arg0, &temp_r4->messageBox, 4);
        ov74_0222A240(arg0, 0);
        *arg1 = 0x14;
        break;
    case 20:
        ov74_0222AB0C(arg0, arg1, (void *)ov74_0222A494);
        break;
    case 21:
        *arg1 = 0x16;
        break;
    case 22:
        temp_r6 = ov74_0222A078(arg0);
        ov74_02235DC4(temp_r4->bgConfig, temp_r6);
        ov74_0222AA18(arg0, &temp_r4->messageBox, 7);
        ov74_02229F28(temp_r4, 1);
        ov74_02229E28(temp_r4, 1);
        temp_r4->delay = 0x3C;
        *arg1 = 0x17;
        break;
    case 23:
        if (--temp_r4->delay != 0) {
            break;
        }
        ov74_0222A174(arg0);
        *arg1 = 0x18;
        break;
    case 24:
        if (ov74_0223615C() == 4) {
            ov74_02236128();
        }
        if (ov74_0223615C() == 2) {
            temp_r4->delay = 1;
            ov74_02229E28(temp_r4, 0);
            ov74_02229F28(temp_r4, -1);
            PlaySE(SEQ_SE_DP_UG_020);
            *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 8, 0x19);
            break;
        }
        if (ov74_0223615C() != 3) {
            break;
        }
        ov74_02229F28(temp_r4, -1);
        temp_r4->eventData.header.unk8E_2 = 0;
        *arg1 = ov74_0222ADBC(arg0, &temp_r4->messageBox, 0x1B, 0x38);
        break;
    case 25:
        if (--temp_r4->delay != 0) {
            break;
        }
        temp_r4->delay = 1 << 16;
        *arg1 = 0x38;
        break;
    case 49:
        if (temp_r4->canReceiveGiftStatus != 5) {
            ov74_0222AE3C(temp_r4);
        }
        *arg1 = ov74_0222AD6C(arg0);
        ov74_02229F60(&temp_r4->menuWindows[2], 0);
        break;
    case 50:
        *arg1 = ov74_0222ADBC(arg0, 0, 0, state);
        break;
    case 51:
        if (gSystem.newKeys == 0) {
            break;
        }
        ov74_02229F60(&temp_r4->wonderCardTitleWindow, 0);
        *arg1 = ov74_0222A5AC(arg0);
        break;
    case 52:
        ov74_0222A240(arg0, 1);
        *arg1 = 8;
        temp_r4->canReceiveGiftStatus = 0;
        break;
    case 53:
        ov74_02229E68(arg0);
        temp_r4->exitToWondercardsApp = 0;
        return 1;
    case 54:
        ov74_02229E68(arg0);
        temp_r4->exitToWondercardsApp = 1;
        return 1;
    case 55:
        ov74_02229E68(arg0);
        temp_r4->exitToWondercardsApp = 2;
        return 1;
    case 56:
        if (gSystem.newKeys == 0) {
            break;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        if (temp_r4->eventData.header.unk8E_2 == 1) {
            ov74_0223539C(0, 0x39, temp_r4->statePtr, 2);
        } else {
            ov74_02235390(1);
            ov74_0223539C(0, 0x3A, temp_r4->statePtr, 2);
        }
        break;
    case 57:
        ov74_0222A7A0(temp_r4->bgConfig);
        GfGfx_EngineBTogglePlanes(1, 0);
        GfGfx_EngineBTogglePlanes(2, 1);
        if (temp_r4->eventData.header.unk8E_2 == 1 && temp_r4->eventData.wonderCard.tag == MG_TAG_ITEM && temp_r4->eventData.wonderCard.data.item == 0x215) {
            ov74_0222FC50(temp_r4->bgConfig, &temp_r4->eventData.wonderCard, 0x54);
        } else {
            ov74_0222EC08(temp_r4->bgConfig, &temp_r4->eventData.wonderCard, 0x54);
        }
        ov74_0223539C(1, 0x38, temp_r4->statePtr, 2);
        temp_r4->eventData.header.unk8E_2 = 0;
        break;
    case 58:
        OS_ResetSystem(0);
        break;
    default:
        break;
    }
    temp_fn = *(s32(**)(void *))&temp_r4->mainCallback;
    if (temp_fn != NULL) {
        temp_fn(temp_r4);
    }
    ov74_022358C8((s32)temp_r4->animationStatus);
    return 0;
}
