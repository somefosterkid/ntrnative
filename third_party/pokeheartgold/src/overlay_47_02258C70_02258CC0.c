#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_wifi_plaza_system.h"

struct MsgData;

struct MessageFormat;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct MsgData MessageLoader;


typedef struct MessageFormat StringTemplate;


typedef struct {
    MessageLoader *unk_00[9];
    StringTemplate *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

void ov47_02258CC0(UnkStruct_ov68_0225CB70 *param0);

void ov47_02258CC0(UnkStruct_ov68_0225CB70 *param0) {
    int v0;

    for (v0 = 0; v0 < 9; v0++) {
        DestroyMsgData(param0->unk_00[v0]);
    }

    MessageFormat_Delete(param0->unk_24);
    String_Delete(param0->unk_28);
    String_Delete(param0->unk_2C);
}



String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2);

String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2) {
    GF_ASSERT(param1 < 9);

    ReadMsgDataIntoString(param0->unk_00[param1], param2, param0->unk_2C);
    StringExpandPlaceholders(param0->unk_24, param0->unk_28, param0->unk_2C);

    return param0->unk_28;
}

String *ov47_02258D18(UnkStruct_ov68_0225CB70 *param0, u32 param1);

String *ov47_02258D18(UnkStruct_ov68_0225CB70 *param0, u32 param1) {
    if (param1 >= (50 + 10)) {
        GF_ASSERT(param1 >= (50 + 10));
        param1 = 0;
    }

    if (param1 < 50) {
        return ov47_02258CEC(param0, 1, param1);
    }

    return ov47_02258CEC(param0, 2, param1 - 50);
}

String *ov47_02258D44(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2);

String *ov47_02258D44(UnkStruct_ov68_0225CB70 *param0, u32 param1, u32 param2) {
    if (param1 >= (50 + 10)) {
        GF_ASSERT(param1 >= (50 + 10));
        param1 = 0;
    }

    if (param1 < 50) {
        return ov47_02258CEC(param0, 3 + param2, param1);
    }

    return ov47_02258CEC(param0, 6 + param2, param1 - 50);
}

struct PlayerProfile;

struct WiFiPlazaProfile;

typedef struct PlayerProfile TrainerInfo;


WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

void ov45_0222A844(const WiFiPlazaProfile *profile, TrainerInfo *param1, enum HeapID heapID);


void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID) {
    TrainerInfo *v0 = PlayerProfile_New(heapID);
    const WiFiPlazaProfile *v1 = ov45_0222A5C0(param0->unk_30);

    ov45_0222A844(v1, v0, heapID);
    BufferPlayersName(param0->unk_24, param1, v0);
    Heap_Free(v0);
}
