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
void ov91_0225CD64(UnkStruct_ov115_02260BE4 *param0);

u8 ov91_0225CD5C(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x37);
}

void ov91_0225CD64(UnkStruct_ov115_02260BE4 *param0) {
    param0->unk_37 = 0;
}
