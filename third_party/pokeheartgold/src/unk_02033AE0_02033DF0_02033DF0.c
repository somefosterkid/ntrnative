#include "global.h"

#include "error_handling.h"
#include "comm_queue_man.h"
#include "comm_ring.h"

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

struct UnkStruct_020322D8_t {
    u8 *unk_00;
    struct UnkStruct_020322D8_t *unk_04;
    struct UnkStruct_020322D8_t *unk_08;
    u16 unk_0C;
    u8 unk_0E;
    u8 unk_0F_0 : 1;
    u8 unk_0F_1 : 1;
    u8 : 6;
};

typedef struct {
    UnkStruct_020322D8 *unk_00;
    UnkStruct_020322D8 *unk_04;
} UnkStruct_02032318;

s32 sub_02033BE4(void *arg0);

s32 sub_02033AF0(CommRing *, u8 *, s32, s32);

s32 sub_02033C28(CommRing *);

UnkStruct_020322D8 *sub_02033C30(CommQueueMan *param0);

s32 sub_020341DC(s32);

s32 sub_02033DF0(CommQueueMan *param0, s32 cmd, u8 *data, s32 size, s32 param4, s32 param5);

s32 sub_02033DF0(CommQueueMan *param0, s32 cmd, u8 *data, s32 size, s32 param4, s32 param5) {
    u8 *sp4;
    s32 sp0;
    CommQueueMan *var_r5;
    UnkStruct_020322D8 *temp_r0;
    UnkStruct_020322D8 *var_r4;
    s32 var_r6;
    s32 var_r7;

    var_r5 = param0;
    sp0 = cmd;
    sp4 = data;
    var_r7 = size;
    var_r4 = sub_02033C30(param0);
    if (var_r4 == NULL) {
        return 0;
    }
    if (var_r7 >= 0xFFFE) {
        GF_AssertFail();
    }
    var_r6 = sub_020341DC(sp0);
    if (var_r6 == 0xFFFF) {
        var_r6 = var_r7;
    }
    if (param5 != 0) {
        if ((s32)(var_r6 + 3) >= sub_02033BE4(var_r5->unk_14)) {
            return 0;
        }
        sub_02033AF0(var_r5->unk_14, sp4, var_r6, 0x109);
        sub_02033C28(var_r5->unk_14);
        *(u8 *)((u8 *)var_r4 + 0xF) = (u8)(2 | *(u8 *)((u8 *)var_r4 + 0xF));
        goto block_10;
    }
block_10:
    var_r4->unk_0C = (u16)var_r6;
    var_r4->unk_0E = (u8)sp0;
    var_r4->unk_00 = sp4;
    temp_r0 = var_r5->unk_00.unk_04;
    if (temp_r0 == NULL) {
        var_r5->unk_00.unk_04 = var_r4;
        var_r5->unk_00.unk_00 = var_r4;
    } else {
        temp_r0->unk_08 = var_r4;
        var_r4->unk_04 = var_r5->unk_00.unk_04;
        var_r5->unk_00.unk_04 = var_r4;
    }
    return 1;
}
