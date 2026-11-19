#include "global.h"

#include "unk_02005D10.h"

typedef struct Ov43Cursor {
    s16 page;
    s16 prevPage;
    s16 pos;
    s16 prevPos;
    u8 unk8[1];
} Ov43Cursor;

typedef struct Ov43Data {
    u8 unk0[0x10];
    u32 count;
} Ov43Data;

void ov43_0222AD40(s32 param_1, u32 param_2, s32 param_3);

void ov43_0222C890(void *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov43_0222BC78(Ov43Cursor *cursor, Ov43Data *data, s32 arg2, s32 action);

s32 ov43_0222BC78(Ov43Cursor *cursor, Ov43Data *data, s32 arg2, s32 action) {
    if (action == 0) {
        PlaySE(SEQ_SE_DP_DECIDE);
        ov43_0222C890(cursor->unk8, arg2, cursor->pos, 1);
        return 3;
    }
    if (action == 5) {
        if (cursor->pos == 8) {
            PlaySE(SEQ_SE_DP_DECIDE);
            ov43_0222C890(cursor->unk8, arg2, cursor->pos, 1);
            return 3;
        }
        if (data->count <= cursor->pos + cursor->page * 8) {
            return 0;
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            ov43_0222C890(cursor->unk8, arg2, cursor->pos, 1);
            return 4;
        }
    } else if (action == 1) {
        ov43_0222AD40(arg2, 0, 1);
        return 1;
    } else if (action == 2) {
        ov43_0222AD40(arg2, 1, 1);
        return 2;
    }
    return 0;
}
