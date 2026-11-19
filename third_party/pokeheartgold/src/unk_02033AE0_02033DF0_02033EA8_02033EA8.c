#include "global.h"

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

typedef struct {
    u8 *buffer;
    s16 startIndex;
    volatile s16 endIndex;
    volatile s16 backupEndIndex;
    s16 size;
} CommRing;

typedef struct CommQueueMan {
    UnkStruct_02032318 unk_00;
    UnkStruct_02032318 unk_08;
    UnkStruct_020322D8 *unk_10;
    CommRing *unk_14;
    void *unk_18;
    int unk_1C;
} CommQueueMan;

BOOL sub_02033C70(UnkStruct_02032318 *param0);

void sub_02033EA8(CommQueueMan *param0);

void sub_02033EA8(CommQueueMan *param0) {

    if (param0->unk_10 != 0L) {
        param0->unk_10 = 0L;
    } else {
        if (!sub_02033C70(&param0->unk_00)) {
            sub_02033C70(&param0->unk_08);
        }
    }
}
