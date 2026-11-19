#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "player_data.h"

struct PlayerProfile;
struct UnkStruct_ov115_02260BE4_t;
void Heap_Free(void *ptr);
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

void ov91_0225CCEC(void *ptr);
BOOL ov91_0225CCF4(UnkStruct_ov115_02260BE4 *param0);

void ov91_0225CCEC(void *ptr) {
    Heap_Free(ptr);
}

BOOL ov91_0225CCF4(UnkStruct_ov115_02260BE4 *param0) {
    int v0;

    if ((param0->unk_00 > 0) && (param0->unk_04 == 1)) {
        param0->unk_00--;
    }

    if (param0->unk_00 <= 0) {
        return 0;
    }

    for (v0 = param0->unk_36; v0 < param0->unk_30->unk_00; v0++) {
        if (param0->unk_30->unk_04[v0].unk_00 >= param0->unk_34) {
            break;
        }
    }

    if (v0 == param0->unk_30->unk_00) {
        v0--;
    }

    if (v0 != param0->unk_36) {
        param0->unk_36 = v0;
        param0->unk_37 = 1;
    }

    return 1;
}
