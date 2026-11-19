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

BOOL ov45_0222E14C(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5);

BOOL ov45_0222E14C(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5) {
    String *v0;

    BufferString(param2, 0, param0->unk_10[0], 0, 1, gGameLanguage);
    BufferString(param2, 1, param0->unk_10[1], 0, 1, gGameLanguage);

    v0 = NewString_ReadMsgData(param3, 7);

    StringExpandPlaceholders(param2, param4, v0);
    String_Delete(v0);

    return 1;
}

BOOL ov45_0222E1A0(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5);

BOOL ov45_0222E1A0(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, MsgData *param3, String *param4, u32 param5) {
    String *v0;

    BufferString(param2, 0, param0->unk_10[0], 0, 1, gGameLanguage);
    BufferString(param2, 1, param0->unk_10[1], 0, 1, gGameLanguage);
    BufferWiFiPlazaInstrumentName(param2, 2, param0->unk_04[0]);

    v0 = NewString_ReadMsgData(param3, 8);

    StringExpandPlaceholders(param2, param4, v0);
    String_Delete(v0);

    return 1;
}
