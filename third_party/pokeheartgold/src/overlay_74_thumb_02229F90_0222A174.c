#include "global.h"

#include "gf_rtc.h"
#include "mystery_gift.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_manager.h"
#include "save.h"
#include "save_arrays.h"
#include "main.h"

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void ov74_022360A0(void *arg0);

extern s32 ov74_0222A130;

void ov74_0222A174(OverlayManager *appMan);

void ov74_0222A174(OverlayManager *appMan) {
    MysteryGiftAppData *temp_r0;
    MysteryGiftEventData *temp_r4;
    u16 *new_var;
    SaveData *sp0;
    s32 var_r6;
    WonderCard *temp_r5;
    MysteryGiftAppData *sp4;
    RTCDate sp8;
    MysteryGiftSave *temp_r0_2;
    temp_r0 = OverlayManager_GetData((OverlayManager *)appMan);
    sp4 = temp_r0;
    temp_r4 = &temp_r0->eventData;
    temp_r5 = &temp_r0->eventData.wonderCard;
    new_var = &temp_r0->eventData.wonderCard.tag;
    sp0 = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(appMan))->saveData;
    if (temp_r4->header.unk8E_2 == 0) {
        var_r6 = 0;
    } else {
        temp_r5->shareCount = 0;
        var_r6 = 1;
        if (temp_r4->header.unk8E_4 == 0) {
            temp_r5->shareMax = 0;
        }
        GF_RTC_CopyDate(&sp8);
        temp_r5->receiveDate = RTC_ConvertDateToDay(&sp8);
    }
    temp_r0_2 = Save_MysteryGift_Get(sp0);
    SaveMysteryGift_ReceivedFlagSet(temp_r0_2, (s32)temp_r4->header.id);
    if (((temp_r4->header.unk8E_2 == 1) && ((*new_var) == 3)) && (temp_r5->data.pokemon.fixedOT == 0x215)) {
        SaveMysteryGift_TrySetSpecialCard(temp_r0_2, temp_r5);
    } else if (var_r6 == 0) {
        SaveMysteryGift_TryInsertGift(temp_r0_2, (MysteryGift *)temp_r5, 3);
    } else {
        SaveMysteryGift_TryInsertCard(temp_r0_2, temp_r5);
    }
    ov74_022360A0(sp0);
    sp4->cancelSave = 0;
    sp4->mainCallback = &ov74_0222A130;
}
