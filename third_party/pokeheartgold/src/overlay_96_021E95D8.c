#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "string_util.h"

s32 ov96_021E95D8(s32 arg0, s32 arg1, s32 arg2);

s32 ov96_021E95D8(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0xA) {
        GF_AssertFail();
    }
    return *(s32 *)((u8 *)(arg0 + (arg1 * 0x10) + (arg2 * 4)) + 0x174);
}

s32 *ov96_021E9344(s32 *param_1);

void ov96_021E95F8(PokeathlonCourseData *param_1, s32 *param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u32 param_7);

void ov96_021E95F8(PokeathlonCourseData *param_1, s32 *param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u32 param_7) {
    enum HeapID heapID;
    MsgData *msgData;
    s32 *piVar1;
    MessageFormat *messageFormat;
    String *src;

    heapID = PokeathlonCourse_GetHeapID(param_1);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    piVar1 = ov96_021E9344(param_2);
    *piVar1 = param_3;
    piVar1[3] = param_4;
    messageFormat = MessageFormat_New(heapID);
    BufferIntegerAsString(messageFormat, (u32)param_5, param_4, (u32)param_6, PRINTING_MODE_RIGHT_ALIGN, 1);
    src = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, param_7, heapID);
    MessageFormat_Delete(messageFormat);
    String_Copy((String *)piVar1[4], src);
    String_Delete(src);
    DestroyMsgData(msgData);
    return;
}

void ov96_021E966C(PokeathlonCourseData *param_1, s32 *param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, u8 param_7, u8 param_8, u8 param_9, u8 param_10, u32 param_11);

void ov96_021E966C(PokeathlonCourseData *param_1, s32 *param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, u8 param_7, u8 param_8, u8 param_9, u8 param_10, u32 param_11) {
    enum HeapID heapID;
    MsgData *msgData;
    s32 *piVar1;
    MessageFormat *messageFormat;
    String *src;

    heapID = PokeathlonCourse_GetHeapID(param_1);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    piVar1 = ov96_021E9344(param_2);
    *piVar1 = param_3;
    piVar1[3] = param_4;
    messageFormat = MessageFormat_New(heapID);
    BufferIntegerAsString(messageFormat, (u32)param_7, param_5, (u32)param_9, PRINTING_MODE_RIGHT_ALIGN, 1);
    BufferIntegerAsString(messageFormat, (u32)param_8, param_6, (u32)param_10, PRINTING_MODE_RIGHT_ALIGN, 1);
    src = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, param_11, heapID);
    MessageFormat_Delete(messageFormat);
    String_Copy((String *)piVar1[4], src);
    String_Delete(src);
    DestroyMsgData(msgData);
    return;
}

void ov96_021E96F8(u32 param_1, u32 param_2, u8 *param_3, PokeathlonCourseData *param_4);

void ov96_021E96F8(u32 param_1, u32 param_2, u8 *param_3, PokeathlonCourseData *param_4) {
    ov96_021E5E04(param_4, param_3);
    PokeathlonCourse_SetStateTransitionType(param_4, 1);
    PokeathlonCourse_SetStateField07(param_4, 1);
    return;
}

s32 ov96_021E8484(PokeathlonCourseData *, u8);

void ov96_021E9718(s32 arg0, u32 arg1, void *arg2, PokeathlonCourseData *arg3);

void ov96_021E9718(s32 index, u32 arg1, void *arg2, PokeathlonCourseData *data) {
    unsigned int new_var;
    u8 v0;
    if (ov96_021E5F24(data) == 0) {
        memcpy(PokeathlonCourse_GetParticipantData(data, index), arg2, arg1);
        PokeathlonCourse_IncrementField1EF(data);
        v0 = PokeathlonCourse_GetParticipantCount(data);
        if (v0 == PokeathlonCourse_GetField1EF(data)) {
            if (((u32)PokeathlonCourse_GetField1EF(data)) < 4U) {
                new_var = 4 - PokeathlonCourse_GetField1EF(data);
                ov96_021E8484(data, (u8)new_var);
            }
            PokeathlonCourse_ResetField1EF(data);
            PokeathlonCourse_SetStateField07(data, 4U);
        }
    }
}

s32 ov96_021E604C(PokeathlonCourseData *);

void ov96_021E9784(s32 arg0, u32 arg1, void *arg2, PokeathlonCourseData *arg3);

void ov96_021E9784(s32 index, u32 arg1, void *arg2, PokeathlonCourseData *data) {
    memcpy(PokeathlonCourse_GetParticipantData(data, index), arg2, arg1);
    ov96_021E604C(data);
    PokeathlonCourse_SetStateTransitionType(data, 3U);
    PokeathlonCourse_SetStateField07(data, 5U);
}

void ov96_021E97B8(u32 param_1, size_t param_2, void *param_3, PokeathlonCourseData *param_4);

void ov96_021E97B8(u32 param_1, size_t param_2, void *param_3, PokeathlonCourseData *param_4) {
    BOOL BVar1;
    PlayerProfile *pPVar2;

    BVar1 = ov96_021E5F24(param_4);
    if (BVar1 != 0) {
        pPVar2 = PokeathlonCourse_GetPlayerProfileFromData(param_4, 0);
        memcpy(pPVar2, param_3, param_2);
    }
    PokeathlonCourse_SetStateTransitionType(param_4, 4);
    PokeathlonCourse_SetStateField07(param_4, 6);
    return;
}
