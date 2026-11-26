#include "global.h"

#include "error_handling.h"

typedef enum {
    UnkEnum_ov66_02231E54_00 = 0,
    UnkEnum_ov66_02231E54_01 = 1 << 0,
    UnkEnum_ov66_02231E54_02 = 1 << 1,
} UnkEnum_ov66_02231E54;

enum WiFiPlazaPlayerEvent {
    WIFI_PLAZA_PLAYER_EVENT_NONE = 0,
    WIFI_PLAZA_PLAYER_EVENT_JOINED,
    WIFI_PLAZA_PLAYER_EVENT_LEFT,
};

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

void ov45_0222DEA4(UnkStruct_ov66_02231D38 *param0, enum WiFiPlazaPlayerEvent param1);

u32 ov45_0222DEE0(const UnkStruct_ov66_02231D38 *param0, u32 param1);
s32 ov45_0222DECC(const UnkStruct_ov66_02231D38 *param0, u32 param1);

void ov45_0222DEA4(UnkStruct_ov66_02231D38 *param0, enum WiFiPlazaPlayerEvent param1) {
    GF_ASSERT(param1 <= UnkEnum_ov66_02231E54_02);
    param0->unk_50 = param1;
}

void ov45_0222DEB8(UnkStruct_ov66_02231D38 *param0, u32 param1);

void ov45_0222DEB8(UnkStruct_ov66_02231D38 *param0, u32 param1) {
    GF_ASSERT(param1 < 20);
    param0->unk_52 = param1;
}

s32 ov45_0222DECC(const UnkStruct_ov66_02231D38 *param0, u32 param1) {
    if (ov45_0222DEE0(param0, param1) != 2) {
        return 1;
    }
    return 0;
}
