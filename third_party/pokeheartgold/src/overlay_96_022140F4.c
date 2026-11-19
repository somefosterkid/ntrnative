#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"

#include "overlay_96_app_02210C98.h"

void ov96_022140F4(PokeathlonCourseData *arg0);

void ov96_022140F4(PokeathlonCourseData *data) {
    MsgData *msgData;
    PokeathlonCourseData *pokeathlonCourseData;
    MessageFormat *messageFormat;
    MsgData *v1;
    String *string;
    s32 index;
    Ov96App_02210C98 *v0;

    pokeathlonCourseData = data;
    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v1 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, v0->unk_58);
    msgData = v1;
    messageFormat = MessageFormat_New(v0->unk_58);
    v0->unk_7B8[0] = NewString_ReadMsgData(v1, 0x133);
    v0->unk_7B8[1] = NewString_ReadMsgData(v1, 0x134);
    v0->unk_7B8[2] = NewString_ReadMsgData(v1, 0x135);
    index = 0;
    do {
        string = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(pokeathlonCourseData, index), v0->unk_58);
        BufferString(messageFormat, 0U, string, 2, 1, 2);
        v0->unk_7C4[index] = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0x132U, v0->unk_58);
        String_Delete(string);
        index = (s32)(u8)(index + 1);
    } while ((u32)index < 4U);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
}

void ov96_022141B0(PokeathlonCourseData *param_1);

void ov96_022141B0(PokeathlonCourseData *param_1) {
    Ov96App_02210C98 *pvVar1;
    u32 uVar2;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    String_Delete(pvVar1->unk_7B8[0]);
    String_Delete(pvVar1->unk_7B8[1]);
    String_Delete(pvVar1->unk_7B8[2]);
    uVar2 = 0;
    do {
#if defined(SDK_PORT) || defined(NONMATCHING)
        String_Delete(pvVar1->unk_7C4[uVar2]);
#else
        String_Delete(*(String **)((s32)pvVar1 + uVar2 * 4 + 0x7c4));
#endif
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 4);
    return;
}
