#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "field_types_def.h"
#include "overlay_01.h"
#include "heap.h"
#include "message_format.h"
#include "player_data.h"
#include "text.h"
#include "unk_0202C034.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_02037C94.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

s32 ov01_021F6C28(void *arg0);

s32 ov01_021F6F94(void *arg0);

s32 ov01_021F71C4(void *, s32);
s32 ov01_021F6FDC(struct UnkStruct_ov5_021EAE78 *arg0);
s32 ov01_021F7060(struct UnkStruct_ov5_021EAE78 *arg0);
s32 ov01_021F6CA0(void *);
s32 sub_0202C338(WiFiList *, s32);
s32 sub_020311AC(FrontierSave *, s32);
s32 ov01_021F7084(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6F94(void *arg0) {
    u16 v0;

    v0 = ((UnkStruct_ov5_021EAE78 *)arg0)->unk_9A;
    switch (v0) {
    case 0:
        ov01_021F6A9C((((UnkStruct_ov5_021EAE78 *)arg0)->fieldSystem), 6, arg0 + 0x94);
        ((UnkStruct_ov5_021EAE78 *)arg0)->unk_9A = (u16)(((UnkStruct_ov5_021EAE78 *)arg0)->unk_9A + 1);
        break;
    case 1:
        if (ov01_021F6C28(arg0) == 1) {
            ((UnkStruct_ov5_021EAE78 *)arg0)->unk_9A = 0U;
            *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_44) = 9;
        }
        break;
    }
    return 0;
}

s32 ov01_021F6FDC(UnkStruct_ov5_021EAE78 *arg0) {
    PlayerProfile *playerProfile;
    WiFiList *a0;
    s32 v0;

    v0 = *((s32 *)&arg0->unk_94);
    switch (v0) {
    case -1:
        break;
    case -2:
        BufferPlayersName(*((MessageFormat * *)&arg0->unk_38), 0U, sub_02034818(*((u32 *)&arg0->unk_6C[7])));
        ov01_021F71C4(arg0, 0x40);
        *((s32 *)&arg0->unk_44) = 6;
        break;
    default:
        *((s32 *)&arg0->unk_8C) = v0;
        a0 = sub_0202C6F4(arg0->saveData);
        playerProfile = PlayerProfile_New(HEAP_ID_FIELD1);
        Save_Profile_PlayerName_Set(playerProfile, sub_0202C254(a0, *((s32 *)&arg0->unk_8C)));
        BufferPlayersName(*((MessageFormat * *)&arg0->unk_38), 0U, playerProfile);
        Heap_Free(playerProfile);
        ov01_021F71C4(arg0, 0x41);
        *((s32 *)&arg0->unk_44) = 0xA;
        break;
    }
    return 0;
}

s32 ov01_021F7060(UnkStruct_ov5_021EAE78 *arg0) {
    if (TextPrinterCheckActive((u8) *((s32 *)&arg0->unk_40)) == 0) {
        *((s16 *)&arg0->unk_98) = 0;
        *((s32 *)&arg0->unk_44) = 0xB;
    }
    return 0;
}

s32 ov01_021F7084(UnkStruct_ov5_021EAE78 *arg0) {
    WiFiList *wiFiList;

    wiFiList = sub_0202C6F4(arg0->saveData);
    if (ov01_021F6CA0(arg0) == 0) {
        if (*((s32 *)&arg0->unk_90) == 0) {
            sub_020311AC(Save_Frontier_GetStatic(arg0->saveData), *((s32 *)&arg0->unk_8C));
            sub_0202C338(wiFiList, *((s32 *)&arg0->unk_8C));
            sub_0203A280(arg0->saveData, *((s32 *)&arg0->unk_6C[7]), 0x1F, HEAP_ID_FIELD1, 0U);
            *((s32 *)&arg0->unk_44) = 1;
        } else {
            BufferPlayersName(*((MessageFormat * *)&arg0->unk_38), 0U, sub_02034818((u32) *((s32 *)&arg0->unk_6C[7])));
            ov01_021F71C4(arg0, 0x40);
            *((s32 *)&arg0->unk_44) = 6;
        }
    }
    return 0;
}
