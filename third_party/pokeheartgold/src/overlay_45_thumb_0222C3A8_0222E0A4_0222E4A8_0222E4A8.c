#include "global.h"

#include "constants/heap.h"
#include "pm_string.h"
#include "overlay_45_wifi_plaza_system.h"
#include "filesystem.h"
#include "message_format.h"

struct MsgData;

struct NARC;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_ov66_02231FB0_t;

typedef u16 charcode_t;

void String_Delete(String *string);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

String *NewString_ReadMsgData(const struct MsgData *loader, u32 entryID);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;




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

BOOL ov45_0222E5B4(enum PlazaMinigame param0, u32 param1);

BOOL ov45_0222E4A8(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, struct MsgData *param3, String *param4, u32 param5);

extern const u16 ov45_02254BD4[4];

BOOL ov45_0222E4A8(UnkStruct_ov66_02231FB0 *param0, const UnkStruct_ov66_02232068 *param1, MessageFormat *param2, struct MsgData *param3, String *param4, u32 param5) {
    String *v0;
    int v2;
    extern const u16 ov45_02254BCA[4];

    if (ov45_0222E5B4((enum PlazaMinigame)(param0->unk_04[0]), param0->unk_04[1]) == 0) {
        return 0;
    }

    BufferWiFiPlazaActivityName(param2, 0, (enum PlazaMinigame)(param0->unk_04[0]));

    for (v2 = 0; v2 < param0->unk_04[1]; v2++) {
        BufferString(param2, v2 + 1, param0->unk_10[v2], 0, 1, gGameLanguage);
    }

    if ((param0->unk_04[0] == UnkEnum_ov66_022324D0_00) || (param0->unk_04[0] == UnkEnum_ov66_022324D0_01)) {
        v0 = NewString_ReadMsgData(param3, ov45_02254BCA[(param0->unk_04[1] - 1) + 1]);
    } else {
        v0 = NewString_ReadMsgData(param3, ov45_02254BD4[param0->unk_04[1] - 1]);
    }

    StringExpandPlaceholders(param2, param4, v0);
    String_Delete(v0);

    return 1;
}
