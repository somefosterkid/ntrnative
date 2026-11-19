#include "global.h"
#include "comm_ring.h"
#include "comm_queue_man.h"
#include "unk_struct_0203233c.h"

int sub_02033BC4(CommRing *ring);
s32 sub_02033BE4(CommRing *ring);
typedef struct UnkStruct_020322D8_t {
    u8 *unk_00;
    struct UnkStruct_020322D8_t *unk_04;
    struct UnkStruct_020322D8_t *unk_08;
    u16 unk_0C;
    u8 unk_0E;
    u8 unk_0F_0 : 1;
    u8 unk_0F_1 : 1;
    u8 : 6;
} UnkStruct_020322D8;
typedef struct {
    UnkStruct_020322D8 *unk_00;
    UnkStruct_020322D8 *unk_04;
} UnkStruct_02032318;
int sub_020341DC(int param0);
BOOL sub_02033C94(UnkStruct_0203233C *param0, u8 param1);
int sub_02033B4C(CommRing *ring, u8 *buffer, int size);
BOOL sub_02033CB0(UnkStruct_020322D8 *param0, UnkStruct_0203233C *param1);
int sub_02033BF4(CommRing *ring);
int sub_02033C14(CommRing *ring, int index);
void sub_02033C28(CommRing *ring);
UnkStruct_020322D8 *sub_02033C30(CommQueueMan *param0);
BOOL sub_02033C50(CommQueueMan *param0);
BOOL sub_02033C70(UnkStruct_02032318 *param0);
BOOL sub_02033C94(UnkStruct_0203233C *param0, u8 param1);
BOOL sub_02033CB0(UnkStruct_020322D8 *param0, UnkStruct_0203233C *param1);
BOOL sub_02033D28(UnkStruct_020322D8 *param0, UnkStruct_0203233C *param1, CommRing *param2, BOOL param3);

s32 sub_02033BE4(CommRing *ring) {
    return ring->size - sub_02033BC4(ring);
}

int sub_02033BF4(CommRing *ring) {
    if (ring->startIndex > ring->backupEndIndex) {
        return ring->startIndex - ring->backupEndIndex;
    }

    return ring->size - (ring->backupEndIndex - ring->startIndex);
}

int sub_02033C14(CommRing *ring, int index) {
    return index % ring->size;
}

void sub_02033C28(CommRing *ring) {
    ring->endIndex = ring->backupEndIndex;
}

UnkStruct_020322D8 *sub_02033C30(CommQueueMan *param0) {
    UnkStruct_020322D8 *v0 = param0->unk_18;
    int v1;

    for (v1 = 0; v1 < param0->unk_1C; v1++) {
        if (v0->unk_0E == 0) {
            return v0;
        }

        v0++;
    }

    return 0L;
}

BOOL sub_02033C50(CommQueueMan *param0) {
    UnkStruct_020322D8 *v0 = param0->unk_18;
    int v1;

    for (v1 = 0; v1 < param0->unk_1C; v1++) {
        if (v0->unk_0E != 0) {
            return 0;
        }

        v0++;
    }

    return 1;
}

BOOL sub_02033C70(UnkStruct_02032318 *param0) {
    if (param0->unk_00 != 0L) {
        if (param0->unk_00->unk_08 != 0L) {
            param0->unk_00 = param0->unk_00->unk_08;
            param0->unk_00->unk_04 = 0L;
        } else {
            param0->unk_00 = 0L;
            param0->unk_04 = 0L;
        }

        return 1;
    }

    return 0;
}

BOOL sub_02033C94(UnkStruct_0203233C *param0, u8 param1) {
    *param0->unk_00 = param1;
    param0->unk_00++;
    param0->unk_04--;

    if (param0->unk_04 == 0) {
        return 1;
    }

    return 0;
}

BOOL sub_02033CB0(UnkStruct_020322D8 *param0, UnkStruct_0203233C *param1) {
    int v0 = sub_020341DC(param0->unk_0E);

    if (v0 == 0xffff) {
        if (param1->unk_04 < 3) {
            param0->unk_0F_0 = 0;
            return 1;
        }
    } else {
        if (param1->unk_04 < 1) {
            param0->unk_0F_0 = 0;
            return 1;
        }
    }

    sub_02033C94(param1, param0->unk_0E);

    if (v0 == 0xffff) {
        sub_02033C94(param1, (param0->unk_0C >> 8) & 0xff);
        sub_02033C94(param1, param0->unk_0C & 0xff);
    } else {
        param0->unk_0C = v0;
    }

    param0->unk_0F_0 = 1;
    return 0;
}

BOOL sub_02033D28(UnkStruct_020322D8 *param0, UnkStruct_0203233C *param1, CommRing *param2, BOOL param3) {
    int v0;
    int v1;
    int v2 = sub_020341DC(param0->unk_0E);

    if (v2 == 0xffff) {
        v1 = 3;
    } else {
        v1 = 1;
    }

    if ((param1->unk_04 < (param0->unk_0C + v1)) && (!param3)) {
        return 0;
    }

    if (param0->unk_0F_0 != 1) {
        if (sub_02033CB0(param0, param1)) {
            return 0;
        }
    }

    if (param1->unk_04 < param0->unk_0C) {
        if (param0->unk_0F_1) {
            sub_02033B4C(param2, param1->unk_00, param1->unk_04);
        } else {
            for (v0 = 0; v0 < param1->unk_04; v0++) {
                param1->unk_00[v0] = param0->unk_00[v0];
            }
        }

        param0->unk_00 += param1->unk_04;
        param0->unk_0C -= param1->unk_04;
        param1->unk_04 = -1;

        return 1;
    }

    if (param0->unk_0F_1) {
        sub_02033B4C(param2, param1->unk_00, param0->unk_0C);
    } else {
        MI_CpuCopy8(param0->unk_00, param1->unk_00, param0->unk_0C);
    }

    param1->unk_00 += param0->unk_0C;
    param1->unk_04 -= param0->unk_0C;

    return 1;
}
