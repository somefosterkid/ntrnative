#include "global.h"

#include "overlay_91.h"
#include "constants/heap.h"
#include "player_data.h"


struct PlayerProfile;
struct UnkStruct_ov115_02260BE4_t;
typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;
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
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;
struct UnkStruct_ov115_02260BE4_t {
    s32 unk_00;
    BOOL unk_04;
    u32 unk_08;
    UnkStruct_ov115_02265AD4 unk_0C;
    u32 unk_1C;
    u32 unk_20[4];
    const UnkStruct_ov115_02262F50 *unk_30;
    u16 unk_34;
    u8 unk_36;
    u8 unk_37;
};
void ov91_0225CDC4(UnkStruct_ov115_02260BE4 *param0, UnkStruct_ov115_02265AD4 *param1);
u32 ov91_0225CDD4(const UnkStruct_ov115_02260BE4 *param0);

u8 ov91_0225CD6C(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x36);
}

void ov91_0225CD74(void *arg0, void *arg1);

void ov91_0225CD74(void *arg0, void *arg1) {
    if (*(u16 *)((u8 *)arg1 + 2) != 0) {
        *(u16 *)((u8 *)arg0 + 0x34) = (u16)(*(u16 *)((u8 *)arg0 + 0x34) + 1);
    }
}

void ov91_0225CD84(void *arg0, u32 arg1, s32 arg2);

void ov91_0225CD84(void *arg0, u32 arg1, s32 arg2) {
    s32 v0;
    u32 v1;

    v1 = arg1;
    if (v1 > 0x1869FU) {
        v1 = 0x1869F;
    }
    v0 = 1 << arg2;
    if (!(*(s32 *)((u8 *)arg0 + 0x1C) & v0)) {
        *(u32 *)((u8 *)(arg0 + (arg2 * 4)) + 0xC) = v1;
        *(s32 *)((u8 *)arg0 + 0x1C) = (s32)(*(s32 *)((u8 *)arg0 + 0x1C) | v0);
    }
}

s32 ov91_0225CDAC(void *arg0) {
    if (ov91_0225CDD4(arg0) >= (u32) * (u32 *)((u8 *)arg0 + 8)) {
        return 1;
    }
    return 0;
}

void ov91_0225CDC4(UnkStruct_ov115_02260BE4 *param0, UnkStruct_ov115_02265AD4 *param1) {
    *param1 = param0->unk_0C;
}

u32 ov91_0225CDD4(const UnkStruct_ov115_02260BE4 *param0) {
    int v0;
    int v1 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        if ((param0->unk_1C & (1 << v0)) != 0) {
            v1++;
        }
    }

    return v1;
}
