#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_45_wifi_plaza_system.h"

typedef struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
} UnkStruct_ov66_02231FB0;

BOOL ov45_0222E414(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5);

BOOL ov45_0222E414(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5) {
    String *v0;
    u32 v1;

    switch (param0->unk_04[1]) {
    case 0:
        BufferWiFiPlazaEventName(param2, 0, 0);
        v1 = 15;
        break;
    case 1:
        BufferWiFiPlazaEventName(param2, 0, 1);
        v1 = 20;
        break;
    case 2:
        BufferWiFiPlazaEventName(param2, 0, 2);
        v1 = 16;
        break;

    case 4:
        v1 = 17;
        break;

    default:
        return 0;
    }

    v0 = NewString_ReadMsgData(param3, v1);

    StringExpandPlaceholders(param2, param4, v0);
    String_Delete(v0);

    return 1;
}

BOOL ov45_0222E484(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5);

BOOL ov45_0222E484(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5) {
    if (param0->unk_04[0] >= 20) {
        return 0;
    }

    if (param1->unk_00[param0->unk_04[0]] == 0) {
        return 0;
    }

    ReadMsgDataIntoString(param3, 21, param4);

    return 1;
}
