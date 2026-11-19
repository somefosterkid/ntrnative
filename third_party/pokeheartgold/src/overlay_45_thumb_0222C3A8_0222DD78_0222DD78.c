#include "global.h"

#include "assert.h"
#include "error_handling.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pm_version.h"

struct MessageFormat;

struct MsgData;

struct NARC;

struct String;

struct UnkStruct_ov45_0222DD38;

struct UnkStruct_ov45_0222DE3C;

struct UnkStruct_ov45_0222DE3C_Slot;

struct UnkStruct_ov45_0222DF78;

struct UnkStruct_ov45_0222E04C;

struct _MAT;

typedef struct UnkStruct_ov45_0222E04C {
    u8 unk0;
    int unk4[3];
    String *unk10[4];
    u16 unk20[4];
    u16 unk28;
    s16 unk2A;
    struct UnkStruct_ov45_0222E04C *unk2C;
    struct UnkStruct_ov45_0222E04C *unk30;
} UnkStruct_ov45_0222E04C;

typedef struct UnkStruct_ov45_0222DF78 {
    UnkStruct_ov45_0222E04C unk0[8];
    UnkStruct_ov45_0222E04C unk1A0;
} UnkStruct_ov45_0222DF78;

typedef struct UnkStruct_ov45_0222DE3C_Slot {
    u16 unk0;
    u16 unk2;
} UnkStruct_ov45_0222DE3C_Slot;

typedef struct UnkStruct_ov45_0222DE3C {
    UnkStruct_ov45_0222DE3C_Slot unk0[20];
    u16 unk50;
    u16 unk52;
} UnkStruct_ov45_0222DE3C;

typedef struct UnkStruct_ov45_0222DD38 {
    u8 unk0[8];
    UnkStruct_ov45_0222DE3C unk8;
    UnkStruct_ov45_0222DF78 unk5C;
} UnkStruct_ov45_0222DD38;

typedef BOOL (*UnkFuncPtr_ov45_0222DD78)(UnkStruct_ov45_0222E04C *, const void *, MessageFormat *, MsgData *, String *, enum HeapID);

extern UnkFuncPtr_ov45_0222DD78 const ov45_02254C0C[9];

BOOL ov45_0222DD44(const UnkStruct_ov45_0222DD38 *a0);

void ov45_0222DF78(UnkStruct_ov45_0222DF78 *a0, enum HeapID heapID);

BOOL ov45_0222E14C(UnkStruct_ov45_0222E04C *a0, const void *a1, MessageFormat *a2, MsgData *a3, String *a4, enum HeapID heapID);

BOOL ov45_0222E1A0(UnkStruct_ov45_0222E04C *a0, const void *a1, MessageFormat *a2, MsgData *a3, String *a4, enum HeapID heapID);

BOOL ov45_0222DD78(const UnkStruct_ov45_0222DD38 *a0, const void *a1, int a2, String *a3, enum HeapID heapID);

BOOL ov45_0222DD78(const UnkStruct_ov45_0222DD38 *a0, const void *a1, int a2, String *a3, enum HeapID heapID) {
    MessageFormat *messageFormat;
    MsgData *msgData;
    UnkStruct_ov45_0222E04C *entry;
    BOOL result;

    ((ov45_0222DD44(a0)) ? (void)0 : GF_AssertFail());

    entry = a0->unk5C.unk1A0.unk2C;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 757, heapID);
    messageFormat = MessageFormat_New(heapID);

    if (entry->unk0 < 9) {
        result = ov45_02254C0C[entry->unk0](entry, a1, messageFormat, msgData, a3, heapID);
    } else {
        result = 0;
    }

    DestroyMsgData(msgData);
    MessageFormat_Delete(messageFormat);
    return result;
}
