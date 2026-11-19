#include "global.h"
struct UnkStruct_ov66_02231FB0_t;

#include "pm_string.h"

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

typedef struct {
    UnkStruct_ov66_02231FB0 unk_00[8];
    UnkStruct_ov66_02231FB0 unk_188;
} UnkStruct_ov66_02231E94;

void ov45_0222E0CC(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1);

UnkStruct_ov66_02231FB0 *ov45_0222E04C(UnkStruct_ov66_02231E94 *param0, u16 param1);

void ov45_0222E094(struct UnkStruct_ov66_02231FB0_t *arg0, struct UnkStruct_ov66_02231FB0_t *arg1);

UnkStruct_ov66_02231FB0 *ov45_0222E04C(UnkStruct_ov66_02231E94 *param0, u16 param1) {
    int v0;
    UnkStruct_ov66_02231FB0 *v1 = NULL;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_00[v0].unk_2C == NULL) {
            v1 = &param0->unk_00[v0];
            break;
        }
    }

    if (v1 == NULL) {
        for (v0 = 0; v0 < 8; v0++) {
            if (param0->unk_00[v0].unk_28 >= param1) {
                ov45_0222E0CC(param0, &param0->unk_00[v0]);
                v1 = &param0->unk_00[v0];
                break;
            }
        }
    }

    return v1;
}

void ov45_0222E094(struct UnkStruct_ov66_02231FB0_t *arg0, struct UnkStruct_ov66_02231FB0_t *arg1) {
    *((void * *)&arg1->unk_2C) = (void *)*((void * *)&arg0->unk_2C);
    *((void * *)&arg1->unk_30) = arg0;
    *((void * *)&arg0->unk_2C) = arg1;
    *(void **)((u8 *)(*((void * *)&arg1->unk_2C)) + 0x30) = arg1;
}
