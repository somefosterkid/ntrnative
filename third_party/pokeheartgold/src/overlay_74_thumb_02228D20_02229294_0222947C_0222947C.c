#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "pm_string.h"
#include "overlay_74_ranger_link_manager.h"

s32 ov74_02235A68(s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02235DC4(void *, MysteryGift *);
#else
s32 ov74_02235DC4(void *, MysteryGift *);
#endif

void ov74_0222947C(RangerLinkManager *arg0);

void ov74_0222947C(RangerLinkManager *arg0) {
    WonderCard *wonderCard = &arg0->manaphyEggWonderCard;
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *string;
    RTCDate date;

    MI_CpuFill8(wonderCard, 0, sizeof(WonderCard));
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, arg0->heapID);
    msgFmt = MessageFormat_New(arg0->heapID);
    wonderCard->tag = 7;
    string = ReadMsgData_ExpandPlaceholders(msgFmt, msgData, 0x4E, arg0->heapID);
    CopyStringToU16Array(string, wonderCard->unk104.name, 36);
    String_Delete(string);
    wonderCard->unk104.version = ov74_02235A68(10) | ov74_02235A68(11) | ov74_02235A68(12) | ov74_02235A68(7) | ov74_02235A68(8);
    wonderCard->unk104.id = 1;
    wonderCard->unk104.unique = 0;
    wonderCard->unk104.unk8E_1 = 0;
    wonderCard->unk104.unk8E_2 = 1;
    wonderCard->unk104.unk8E_3 = 1;
    wonderCard->unk104.unk8E_5 = 0;
    string = ReadMsgData_ExpandPlaceholders(msgFmt, msgData, 0x4D, arg0->heapID);
    CopyStringToU16Array(string, wonderCard->text, 250);
    String_Delete(string);
    wonderCard->shareMax = 0;
    wonderCard->monIcon[0] = 0x1EA;
    wonderCard->monIcon[1] = 0;
    wonderCard->monIcon[2] = 0;
    wonderCard->shareCount = 0;
    GF_RTC_CopyDate(&date);
    wonderCard->receiveDate = RTC_ConvertDateToDay(&date);
    MessageFormat_Delete(msgFmt);
    DestroyMsgData(msgData);
    SaveMysteryGift_TryInsertGift(arg0->mysteryGift, (MysteryGift *)wonderCard, 3);
    GfGfx_EngineBTogglePlanes(1, 0);
    ov74_02235DC4(arg0->bgConfig, (MysteryGift *)wonderCard);
}
