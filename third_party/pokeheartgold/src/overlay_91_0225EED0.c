#include "global.h"
#include "sys_task.h"
#include "player_data.h"

struct PlayerProfile;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov115_022612BC;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;
typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;
typedef struct {
    UnkStruct_ov115_02262DC4 unk_00;
    UnkStruct_ov115_02262DC4 unk_18;
    UnkStruct_ov115_02262DC4 unk_30;
    s32 unk_48;
} UnkStruct_ov115_02262E64;
void ov91_0225F05C(const UnkStruct_ov115_02262F50 *param0, const UnkStruct_ov115_022612BC *param1, u32 param2, UnkStruct_ov115_02262DC4 *param3);

void ov91_0225EED0(UnkStruct_ov115_02262E64 *param0, const UnkStruct_ov115_02262F50 *param1, const UnkStruct_ov115_022612BC *param2, u16 param3, u16 param4);
BOOL ov91_0225EF70(UnkStruct_ov115_02262E64 *param0);

SysTask *ov91_0225F010(SysTaskFunc callback, void *param, u32 priority);
SysTask *ov91_0225F008(SysTaskFunc callback, void *param, int priority);

void ov91_0225EED0(UnkStruct_ov115_02262E64 *param0, const UnkStruct_ov115_02262F50 *param1, const UnkStruct_ov115_022612BC *param2, u16 param3, u16 param4) {
    UnkStruct_ov115_02262DC4 v0, v1;

    ov91_0225F05C(param1, param2, param3, &v0);
    ov91_0225F05C(param1, param2, param4, &v1);

    param0->unk_18 = v0;
    param0->unk_00 = v0;
    param0->unk_30.unk_00 = v1.unk_00 - v0.unk_00;
    param0->unk_30.unk_08 = v1.unk_08 - v0.unk_08;
    param0->unk_30.unk_10 = v1.unk_10 - v0.unk_10;
    param0->unk_30.unk_14 = v1.unk_14 - v0.unk_14;

    if ((v1.unk_04 - v0.unk_04) < ((0xffff + v1.unk_04) - v0.unk_04)) {
        param0->unk_30.unk_04 = v1.unk_04 - v0.unk_04;
    } else {
        param0->unk_30.unk_04 = ((0xffff + v1.unk_04) - v0.unk_04);
    }

    if ((v1.unk_0C - v0.unk_0C) < ((0xffff + v1.unk_0C) - v0.unk_0C)) {
        param0->unk_30.unk_0C = v1.unk_0C - v0.unk_0C;
    } else {
        param0->unk_30.unk_0C = ((0xffff + v1.unk_0C) - v0.unk_0C);
    }

    param0->unk_48 = 0;
}

BOOL ov91_0225EF70(UnkStruct_ov115_02262E64 *param0) {
    if (param0->unk_48 >= 16) {
        return 1;
    }

    param0->unk_48++;

    param0->unk_00.unk_00 = param0->unk_18.unk_00 + ((param0->unk_30.unk_00 * param0->unk_48) / 16);
    param0->unk_00.unk_04 = param0->unk_18.unk_04 + ((param0->unk_30.unk_04 * param0->unk_48) / 16);
    param0->unk_00.unk_08 = param0->unk_18.unk_08 + ((param0->unk_30.unk_08 * param0->unk_48) / 16);
    param0->unk_00.unk_0C = param0->unk_18.unk_0C + ((param0->unk_30.unk_0C * param0->unk_48) / 16);
    param0->unk_00.unk_10 = param0->unk_18.unk_10 + ((param0->unk_30.unk_10 * param0->unk_48) / 16);
    param0->unk_00.unk_14 = param0->unk_18.unk_14 + ((param0->unk_30.unk_14 * param0->unk_48) / 16);

    return 0;
}

SysTask *ov91_0225F008(SysTaskFunc callback, void *param, int priority) {
    return ov91_0225F010(callback, param, priority);
}
