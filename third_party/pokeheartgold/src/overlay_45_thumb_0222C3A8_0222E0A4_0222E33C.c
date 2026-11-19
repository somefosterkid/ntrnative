#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "overlay_45_wifi_plaza_system.h"

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
};

s32 ov45_0222E33C(UnkStruct_ov66_02231FB0 *param0, UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5);

s32 ov45_0222E33C(UnkStruct_ov66_02231FB0 *param0, UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5) {
    String *string;

    BufferWiFiPlazaActivityName((MessageFormat *)param2, 0U, param0->unk_04[0]);
    BufferString((MessageFormat *)param2, 1U, param0->unk_10[0], 0, 1, (s32)gGameLanguage);
    BufferIntegerAsString((MessageFormat *)param2, 2U, 8 - param0->unk_04[1], 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    string = NewString_ReadMsgData((MsgData *)param3, 9);
    StringExpandPlaceholders((MessageFormat *)param2, param4, string);
    String_Delete(string);
    return 1;
}
