#include "global.h"

#include "constants/heap.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_45_wifi_plaza_system.h"
#include "bg_window.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "message_format.h"
#include "filesystem_files_def.h"
#include "msgdata.h"

struct ListMenu2D;

struct MsgData;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteResource;

struct String;

struct MessageFormat;

struct Window;

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

typedef struct ListMenu2D ListMenu2D;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[1];
    s32 unk_10;
    Sprite *unk_14;
    SpriteResource *unk_18[4];
    u16 unk_28;
    u16 unk_2A;
    UnkStruct_ov68_0225DB8C unk_2C;
} UnkStruct_ov68_0225D0F8;

typedef struct {
    Window unk_00[6];
    u16 unk_60;
    u16 unk_62;
    s32 unk_64;
    s32 unk_68[3];
    s32 unk_74[3];
    void *unk_80;
    NNSG2dCharacterData *unk_84;
    UnkStruct_ov68_0225DB8C unk_88;
    UnkStruct_ov68_0225DB8C unk_A8;
    UnkStruct_ov68_0225DC74 unk_C8;
    UnkStruct_ov68_0225DC74 unk_DC;
} UnkStruct_ov68_0225D388;

typedef union {
    UnkStruct_ov68_0225D0F8 val1;
    UnkStruct_ov68_0225D388 val2;
} UnkUnion_ov68_0225CCA8;

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

void ov47_02258A1C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov47_02258AA0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);

BOOL sub_0207217C(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259488(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);

BOOL sub_02072190(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_022599F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_02259C8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

void ov47_02258C70(UnkStruct_ov68_0225CB70 *param0, WiFiPlazaSystem *param1, enum HeapID heapID);

void ov47_02258C70(UnkStruct_ov68_0225CB70 *param0, WiFiPlazaSystem *param1, enum HeapID heapID) {
    int v0;
    static const v1[9] = {
        0x309,
        0x2F9,
        0x2FD,
        0x2F6,
        0x2F7,
        0x2F8,
        0x2FA,
        0x2FB,
        0x2FC,
    };

    param0->unk_30 = param1;

    for (v0 = 0; v0 < 9; v0++) {
        param0->unk_00[v0] = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (enum NarcId)27, v1[v0], heapID);
    }

    param0->unk_24 = MessageFormat_New(heapID);
    param0->unk_28 = String_New(256, heapID);
    param0->unk_2C = String_New(256, heapID);
}
